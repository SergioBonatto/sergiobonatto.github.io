# Auditoria Técnica: `src/`, `include/` e `tools/`

## Escopo e critério

Esta auditoria cobre todos os arquivos em `src/`, `include/` e `tools/`, com foco em:

- qualidade de código e coesão arquitetural;
- eficiência de memória, com ênfase em redução de heap e pressão de cópia;
- escalabilidade do modelo atual para mais posts, páginas e componentes;
- performance em runtime no navegador e no host-side tooling;
- riscos de segurança, especialmente buffer overflow, injeção de HTML/atributos e inconsistências de parsing;
- oportunidades específicas para WebAssembly, incluindo layout de memória, crossing WASM/JS, binary size e vetorização.

O projeto é tecnicamente pequeno, mas há alguns problemas estruturais que hoje já afetam corretude, robustez e custo de manutenção. O principal ponto: o código evita alocação frequente no runtime WASM, o que é bom, porém substitui isso por buffers globais grandes, parsing permissivo demais e algumas fronteiras JS/HTML sem sanitização adequada.

---

## Resumo executivo

### Pontos fortes

- O runtime principal evita heap no caminho quente de renderização; isso é apropriado para WASM e reduz GC/fragmentação.
- O parser de Markdown trabalha em streaming sobre `const char *`, sem construir ASTs intermediárias.
- O pipeline de empacotamento em `tools/packer.c` move custo de ingestão de conteúdo para build-time, o que é coerente com a proposta do engine.
- O código é pequeno e relativamente legível, o que torna viável uma refatoração de alto impacto com baixo custo absoluto.

### Problemas mais relevantes

1. O output HTML é gerado com interpolação insegura em vários pontos. Há risco real de injeção por atributos e conteúdo não escapado em `src/ui.c` e `tools/packer.c`.
2. O parser Markdown contém leituras fora do intervalo lógico do slice em caminhos de texto matemático e shortcode de gráfico. Isso não explode sempre porque o conteúdo está NUL-terminated, mas o contrato por `len` está violado e a lógica fica incorreta em inputs malformados.
3. O buffer global de 512 KiB em `include/buffer.h` força um piso alto de memória linear em WASM e falha por overflow silencioso sem backpressure nem fallback.
4. O bridge JS/WASM em `src/js_api.c` recria strings UTF-8, faz `innerHTML` para renders completos e aloca com `_malloc/_free` em cada `hashchange`; isso aumenta custo de crossing e cria dependência desnecessária do allocator exportado.
5. O empacotador gera conteúdo como arrays hexadecimais em C, o que tende a inflar tempo de compilação e tamanho do binário final; além disso há vulnerabilidades e truncamentos silenciosos na geração de slugs e metadados.

### Prioridade sugerida

1. Fechar superfície de injeção/sanitização.
2. Corrigir parsing bounded por `len`.
3. Substituir buffer global monolítico por modelo com capacidade configurável e erro explícito.
4. Reduzir crossings JS/WASM e custo de `innerHTML`.
5. Reestruturar o packer para gerar blobs/string tables mais compactos.

---

## Achados críticos e altos

### 1. Injeção de HTML/atributos no renderer

**Arquivos**: [src/ui.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/ui.c:17), [src/ui.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/ui.c:33), [src/ui.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/ui.c:45), [src/js_api.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/js_api.c:13), [src/js_api.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/js_api.c:76), [tools/packer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/tools/packer.c:189)

**Problema**

- `add_code_block()` insere `lang` em `class="language-%.*s"` sem escaping.
- `add_image()` insere `path` e `alt` em atributos `data-*` sem escaping.
- `add_blog_entry()` insere `date`, `slug` e `title` diretamente em HTML.
- `sys_set_html()` e `sys_append_child()` usam `innerHTML` como sink final.
- `tools/packer.c` escreve `title`, `date`, `slug` e `description` em código C sem escaping de aspas, barras invertidas ou novas linhas.

**Impacto**

- Um markdown com `alt="x" onmouseover="..."` ou front matter com aspas pode quebrar o HTML gerado.
- Em conteúdo controlado pelo autor isso ainda é um bug de robustez; em conteúdo syncado ou importado de outras fontes, vira vetor de XSS armazenado.
- No packer, metadados com `"` ou `\` podem gerar C inválido ou conteúdo truncado/corrompido no build.

**Justificativa**

Hoje o projeto depende implicitamente da premissa “todo conteúdo é totalmente confiável e bem-formado”. Essa premissa não escala. Mesmo em sites pessoais, erros de escaping quebram deploys e deixam o pipeline frágil.

**Recomendação**

- Introduzir um `buf_append_attr_escaped()` separado de `buf_escape()`, escapando pelo menos `&`, `<`, `>`, `"`, `'`.
- Em `add_blog_entry()`, deixar de gerar `onclick` inline; gerar apenas `href` e resolver navegação por delegation no JS.
- No packer, adicionar rotina `c_escape_string()` para emitir strings C válidas.
- Se o projeto quiser manter `innerHTML`, toda string que cruza a fronteira precisa estar canonicalmente escapada antes.

**Prioridade**: crítica.

### 2. Parsing de matemática ignora limite `len`

**Arquivo**: [src/markdown.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/markdown.c:81)

**Problema**

- Em modo display, `render_text()` usa `strstr(p, "$$")` em vez de uma busca limitada por `end`.
- Em modo inline, usa `strchr(p, '$')`, igualmente sem respeito ao final lógico do slice.

**Impacto**

- Se `render_text()` receber um recorte que não termina no fim da string original, a busca pode atravessar o fim lógico e capturar delimitadores de outra região.
- Isso cria comportamento incorreto e torna o contrato `const char * + len` inconsistente.
- Em parsers incrementais ou futuras fontes de conteúdo não NUL-terminated, isso vira leitura fora dos limites válidos.

**Justificativa**

O parser já foi desenhado para trabalhar com slices (`text`, `len`). Romper esse contrato em um subcaminho mina previsibilidade e dificulta qualquer otimização posterior.

**Recomendação**

- Substituir `strstr`/`strchr` por `memchr`/scanner manual bounded.
- Padronizar toda a pipeline de parsing para operar em `(ptr, end)`, nunca em C-string implícita.

**Prioridade**: alta.

### 3. Leitura fora do intervalo em shortcode `[[graph:...]]`

**Arquivo**: [src/markdown.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/markdown.c:176)

**Problema**

- Após `memchr(line, ']', len)`, o código acessa `p[1]` sem garantir que `p < line + len - 1`.

**Impacto**

- Em linha terminada por `]` simples, `p[1]` acessa um byte fora do range semântico do slice.
- Na prática pode ler o `\0` vizinho e “funcionar”, mas o código depende de layout incidental.

**Recomendação**

- Validar `p && (size_t)(p - line + 1) < len` antes de testar `p[1]`.
- Melhor ainda: usar um parser explícito com `end`.

**Prioridade**: alta.

### 4. Truncamento/overflow de slug no packer

**Arquivo**: [tools/packer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/tools/packer.c:12), [tools/packer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/tools/packer.c:159)

**Problema**

- `strncpy(p->slug, de->d_name, MAX_SLUG - 1)` não garante terminador se `d_name` tiver tamanho `>= MAX_SLUG - 1`.
- `sanitize_slug()` não recebe capacidade de destino e escreve até o fim de `src`.
- Em seguida há `strcpy(p->slug, c_slug)`.

**Impacto**

- Um nome de arquivo longo pode causar leitura além do buffer em `strstr(p->slug, ".md")`.
- `sanitize_slug()` pode estourar `c_slug`.
- `strcpy()` replica o problema para `p->slug`.

**Justificativa**

Esse é o ponto de entrada do conteúdo. Mesmo em ambiente controlado, ferramentas de build não devem confiar em nomes curtos.

**Recomendação**

- Redefinir `sanitize_slug(char *dst, size_t dst_cap, const char *src)`.
- Trocar `strncpy`/`strcpy` por `snprintf` ou cópia bounded explícita.
- Detectar truncamento e abortar o build com erro descritivo.

**Prioridade**: alta.

### 5. `extract_value()` tem underflow lógico em string vazia

**Arquivo**: [tools/packer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/tools/packer.c:55)

**Problema**

- `char *end = dest + strlen(dest) - 1;` é avaliado mesmo quando `dest` fica vazia.

**Impacto**

- Para metadado vazio, `end` passa a apontar antes de `dest`. O loop não executa, mas o ponteiro já ficou fora do objeto, o que é UB em C estrito.

**Recomendação**

- Calcular `size_t n = strlen(dest); if (n == 0) return;`.

**Prioridade**: alta.

### 6. Buffer global grande com overflow silencioso

**Arquivos**: [include/buffer.h](/Users/bonatto/Work/Projects/sergiobonatto.github.io/include/buffer.h:8), [src/buffer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/buffer.c:14), [src/ui.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/ui.c:75)

**Problema**

- `BUFFER_CAPACITY` fixa 512 KiB em memória estática.
- Em overflow, o buffer só seta `overflow = true`; `ui_end_render()` ignora esse estado e envia HTML parcial ao DOM.

**Impacto**

- A memória linear inicial do módulo sobe mesmo em páginas pequenas.
- Conteúdo maior que o buffer falha de forma silenciosa e possivelmente gera DOM inválido.
- O design não escala bem para posts longos nem para futuro suporte a listas/tabelas maiores.

**Justificativa**

Evitar heap é válido, mas um buffer monolítico global não é a única alternativa. Em WASM, 512 KiB estáticos impactam initial memory, download indirect cost e pressure sobre caches.

**Recomendação**

- Mínimo: propagar erro quando `overflow` ocorrer e renderizar fallback visível.
- Melhor: usar arena linear por render ou buffer chunked append-only.
- Melhor ainda: separar “HTML shell” e “content body” para evitar recompor tudo em um único bloco.

**Prioridade**: alta.

---

## Achados médios

### 7. O runtime evita heap no caminho quente, mas o bridge JS reintroduz churn de alocação

**Arquivo**: [src/js_api.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/js_api.c:92)

**Problema**

- Em cada `hashchange`, o JS faz `_malloc(len)`, `stringToUTF8`, chama `_handle_route`, depois `_free`.

**Impacto**

- O motor principal se posiciona como “zero-allocation” no parsing, mas o bridge de navegação cria alocação por evento.
- O custo absoluto é pequeno hoje, porém o padrão é ruim e se espalha com facilidade.

**Recomendação**

- Exportar uma função C que leia o hash diretamente em buffer estático chamando `sys_get_url_hash`.
- Ou usar `stackAlloc`/`stackSave`/`stackRestore` do lado JS em vez de `_malloc/_free`, removendo dependência do allocator exportado.

**Prioridade**: média.

### 8. `innerHTML` full replace em cada navegação custa parse de DOM desnecessário

**Arquivos**: [src/ui.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/ui.c:75), [src/js_api.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/js_api.c:13)

**Problema**

- Toda troca de página reenvia o HTML inteiro de `#feed` como string, forçando parse completo, destruição de nós antigos e reinstalação dos placeholders de imagem.

**Impacto**

- Para poucos posts é aceitável; com crescimento do blog vira o principal gargalo percebido.
- Também dificulta transições incrementais, virtualização e preservação de estado DOM.

**Recomendação**

- Curto prazo: usar `replaceChildren(fragment)` gerado no JS a partir de tokens/estruturas simples.
- Médio prazo: modelar o output como comandos de render (“paragraph”, “image”, “code block”, “blog row”) em vez de HTML bruto.
- Isso reduz a superfície de XSS e torna o bridge mais “hardware-inspired” de fato.

**Prioridade**: média.

### 9. `render_tick()` redesenha a tela inteira sem necessidade

**Arquivo**: [src/main.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/main.c:18), [src/js_api.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/js_api.c:223)

**Problema**

- O loop principal roda continuamente e redesenha fundo + cabeçalho em todo frame.
- O valor `t` nem é utilizado em `draw_frame()`.

**Impacto**

- CPU/GPU ficam trabalhando mesmo quando a UI está estática.
- Em dispositivos móveis isso consome bateria e reduz responsividade de scrolling.

**Recomendação**

- Se a renderização do canvas é estática, trocar para render sob demanda: inicialização, resize, theme toggle, mudança de texto.
- Se quiser manter animation-ready, ao menos usar `emscripten_set_main_loop_timing(EM_TIMING_RAF, 1)` explicitamente e invalidar apenas quando houver dirty flag.

**Prioridade**: média.

### 10. `state.page` fica inconsistente na rota 404

**Arquivo**: [src/router.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/router.c:13)

**Problema**

- `switch_to_404()` renderiza a página 404, mas não seta `state.page = PAGE_404`.

**Impacto**

- O estado interno deixa de refletir a UI real.
- Isso vira bug assim que aparecer lógica dependente do estado atual.

**Recomendação**

- Atualizar estado antes de renderizar.

**Prioridade**: média.

### 11. `open_article_by_slug()` faz busca linear

**Arquivo**: [src/router.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/router.c:39)

**Problema**

- Cada navegação por slug percorre `posts[]` linearmente.

**Impacto**

- Com poucos posts é irrelevante; com centenas ou milhares, a latência cresce sem necessidade.

**Recomendação**

- Como o conteúdo é build-time, gerar no packer um índice ordenado por slug e usar busca binária.
- Alternativamente, gerar perfect hash ou trie compacta se a prioridade for lookup constante e binary size previsível.

**Prioridade**: média.

### 12. `embedded_posts[]` é gerado e nunca usado

**Arquivo**: [tools/packer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/tools/packer.c:199)

**Problema**

- O packer gera `embedded_posts[]`, mas `get_article_body()` usa `switch(index)` e não consulta essa tabela.

**Impacto**

- A tabela ocupa bytes no binário e complica leitura do arquivo gerado sem benefício.

**Recomendação**

- Remover `embedded_posts[]` ou passar a usá-lo de verdade.

**Prioridade**: média.

### 13. O parser de matemática é correto apenas para um subconjunto muito estreito

**Arquivo**: [src/math.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/math.c:185)

**Problema**

- Só aceita um único `^` ou `_` logo após o átomo, não compõe `msubsup`, não trata comandos desconhecidos de forma renderizável, e usa cópia temporária fixa de 512 bytes para reescrever o átomo base.

**Impacto**

- Expressões legítimas vão degradar silenciosamente.
- `atom_len >= 512` faz o parser simplesmente deixar de aplicar sup/sub, sem sinalização.

**Recomendação**

- Trocar a estratégia de “emitir e depois rebobinar” por parsing recursivo com nó lógico mínimo: `base`, `sub`, `sup`.
- Se quiser manter zero-heap, usar uma pequena struct temporária por frame de parser, não cópia de HTML já serializado.

**Prioridade**: média.

### 14. `match_str()` é código morto

**Arquivo**: [src/math.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/math.c:29)

**Impacto**

- Pequeno, mas adiciona ruído e bytes.

**Recomendação**

- Remover.

**Prioridade**: média.

### 15. `add_named_graph()` é declarado mas não implementado

**Arquivo**: [include/ui.h](/Users/bonatto/Work/Projects/sergiobonatto.github.io/include/ui.h:108)

**Impacto**

- A API pública sugere uma feature inexistente.
- Isso induz chamadas erradas e degrada a confiabilidade das headers como contrato.

**Recomendação**

- Implementar ou remover da interface pública.

**Prioridade**: média.

### 16. Duplicação de fronteira de plataforma em `include/js_api.h` e `include/sys.h`

**Arquivos**: [include/js_api.h](/Users/bonatto/Work/Projects/sergiobonatto.github.io/include/js_api.h:8), [include/sys.h](/Users/bonatto/Work/Projects/sergiobonatto.github.io/include/sys.h:10)

**Problema**

- Há duas headers para a mesma camada, com responsabilidades sobrepostas.

**Impacto**

- Facilita divergência de assinaturas e espalha acoplamento com Emscripten.

**Recomendação**

- Consolidar em uma única interface de plataforma.
- Reservar `render.h` para primitives visuais e `sys.h` para bindings do host.

**Prioridade**: média.

### 17. `page_add_footer()` depende de `localtime()`

**Arquivo**: [src/pages.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/pages.c:41)

**Problema**

- Em WASM/browser, `time()` e `localtime()` podem puxar runtime support desnecessário.

**Impacto**

- Não é gargalo, mas adiciona dependência que poderia ser resolvida em JS ou build-time.

**Recomendação**

- Como o footer não muda em sessão, gerar o ano em JS ou build-time.
- Isso simplifica o lado C e reduz dependência de libc/timezone plumbing.

**Prioridade**: média.

---

## Observações por arquivo

### `src/main.c`

- [src/main.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/main.c:18): `state.runtime` é incrementado, mas `draw_frame()` não usa `t`. Isso é trabalho morto.
- [src/main.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/main.c:38): o caminho inicial para `"#/"` apenas atualiza URL e não renderiza explicitamente a home. Isso parece depender do DOM inicial já conter o conteúdo esperado, o que é frágil.

### `src/buffer.c` / `include/buffer.h`

- A interface é simples e adequada para append-only.
- Falta API para consultar/propagar erro formalmente.
- [src/buffer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/buffer.c:34): `remaining` é `int`, mas deriva de `size_t`; hoje não estoura porque o buffer é pequeno, mas o tipo é desnecessariamente estreito.
- `buf_escape()` chama `buf_append()` para cada entidade, o que recalcula `strlen()` em strings constantes repetidas; é pequeno, mas em conteúdo grande é custo evitável. Melhor usar `buf_append_len()` com comprimentos constantes.

### `src/config.c` / `include/config.h`

- Configuração centralizada ajuda.
- A maioria das strings CSS/HTML está em C, o que aumenta binary size e torna alteração visual custosa.
- [src/config.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/config.c:104): `footer_style` é na prática um template HTML, não “style”. O nome induz erro conceitual.
- `palette` e `theme` usam `int`; para WASM e cache locality, `uint8_t` bastaria.

### `src/js_api.c` / `include/js_api.h` / `include/sys.h` / `include/render.h`

- O arquivo concentra plataforma, render e UI bootstrap; isso prejudica coesão.
- [src/js_api.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/js_api.c:154): leitura manual do layout de `struct theme` via offsets em JS é frágil. Se o layout mudar, a camada quebra silenciosamente.
- [src/js_api.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/js_api.c:178): atualizar 16 custom properties toda vez que o tema troca é aceitável, mas poderia ser substituído por troca de classe no root para reduzir JS.
- [src/js_api.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/js_api.c:199): resize seta `canvas.width/height` diretamente para `innerWidth/innerHeight`; em telas high-DPI isso perde nitidez. Se o canvas permanecer, usar `devicePixelRatio`.

### `src/markdown.c` / `include/markdown.h`

- O parser é de baixa alocação e fácil de seguir.
- Ele opera por linhas, então não suporta listas, blockquotes, nested structures nem paragraphs multi-line reais; isso limita a escalabilidade semântica do formato.
- [src/markdown.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/markdown.c:200): front matter é detectado por contagem cega de `---`; uma linha horizontal no corpo pode alternar o estado indevidamente.
- [src/markdown.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/markdown.c:214): fence de código não valida indentação nem linguagem.
- [src/markdown.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/src/markdown.c:38): `render_graph_shortcode()` usa stack arrays pequenos, o que é bom, mas falta validação de intervalos (`pct`, `opacity`, dimensões).

### `src/math.c` / `include/math.h`

- Boa escolha encapsular MathML em módulo separado.
- O parser serializa diretamente em HTML, o que economiza memória, mas amarra parsing e rendering.
- `isspace`, `isdigit`, `isalpha` recebem `char` potencialmente assinado; para bytes >127 isso é UB. Deve-se converter para `(unsigned char)`.

### `src/pages.c` / `include/pages.h`

- Estrutura simples.
- O modelo atual mistura decisão de SEO, layout e conteúdo na mesma função. Com mais páginas isso tende a duplicar padrões.
- O loop do blog re-renderiza todos os posts sempre; se a lista crescer, convém paginação ou chunking.

### `src/router.c` / `include/router.h`

- Router minimalista e claro.
- `load_article()` aparece declarado em `router.h`, mas pertence semanticamente ao subsistema de conteúdo/markdown. A header está misturando domínios.

### `src/ui.c` / `include/ui.h`

- Bom encapsulamento do buffer para a maior parte do projeto.
- A API pública mistura primitives de render, navegação, SEO e componentes concretos.
- Recomendo separar em:
  - `ui_render.h`: paragraphs, code block, image, graph;
  - `ui_nav.h`: router/theme/nav;
  - `ui_meta.h`: SEO/meta/footer.

### `tools/packer.c` / `tools/packer.h`

- O utilitário cumpre o papel de build-time ingestion.
- O custo de memória no host é irrelevante, então aqui vale priorizar robustez e qualidade do output, não “zero allocation”.
- [tools/packer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/tools/packer.c:151): filtro por `strstr(de->d_name, ".md")` aceita nomes como `foo.md.bak`.
- [tools/packer.c](/Users/bonatto/Work/Projects/sergiobonatto.github.io/tools/packer.c:124): ordenação por `strcmp()` em datas só é válida porque o formato presumido é ISO `YYYY-MM-DD`; o tool deveria validar isso.

---

## Eficiência de memória

### Situação atual

- Runtime WASM:
  - evita heap no parsing e renderização;
  - mantém um buffer global de 512 KiB;
  - usa pequenos buffers de stack (`initial_hash[256]`, `footer_html[1024]`, arrays do graph, `atom_tmp[512]`).
- Tooling host:
  - lê arquivo inteiro com `malloc`;
  - duplica o conteúdo com `strdup` para parsear metadados.

### Avaliação

O runtime está mais próximo de “single static arena” do que de “zero-allocation”. Isso continua sendo uma boa direção para WASM, mas a implementação atual sacrifica previsibilidade:

- a reserva global é fixa e pessimista;
- o erro de capacidade não é tratado;
- o custo de `innerHTML` faz cópias adicionais no lado JS de qualquer forma.

### Recomendações

1. Trocar `Buffer g_html_buf` por `RenderArena` com:
   - ponteiro;
   - capacidade;
   - comprimento;
   - flag de erro;
   - opcionalmente `peak_len` para telemetria.

   Justificativa: permite dimensionar memória com dados reais e detectar regressões de footprint.

2. Se o objetivo for heap zero estrito, usar:
   - arena estática menor para página comum, por exemplo 64-128 KiB;
   - fallback controlado para “content too large”.

   Justificativa: posts pessoais raramente exigem 512 KiB de HTML serializado ao mesmo tempo.

3. No packer, evitar `strdup(buf)`:
   - parsear front matter numa única passada sobre `buf`;
   - ou ler apenas o prefixo do arquivo até fechar o segundo `---`.

   Justificativa: reduz cópia e melhora tempo de build em lotes maiores.

4. Em `struct theme`, trocar `int` por `uint8_t`.

   Justificativa: os índices cabem em 0..15; em arrays de temas maiores isso melhora packing e reduz tráfego de memória.

---

## Performance e escalabilidade

### Gargalos atuais

1. Full DOM replace via `innerHTML`.
2. Busca linear de slug.
3. Redraw contínuo do canvas.
4. Parsing Markdown linha a linha sem representação intermediária, o que impede reuse/parcial update.

### Melhorias arquiteturais

#### A. Substituir HTML string por protocolo de render

Emitir comandos como:

```c
enum op {
	OP_P,
	OP_H,
	OP_CODE,
	OP_IMAGE,
	OP_BLOG_ROW,
	OP_BAR
};
```

E no JS, construir DOM por `document.createElement`.

**Justificativa**

- reduz XSS;
- reduz custo de parse de HTML;
- abre caminho para partial updates;
- aproxima a ideia de “UI bridge de baixa latência” de um modelo de comandos explícitos.

#### B. Pré-indexar posts no build

Gerar:

- vetor ordenado por data para o índice;
- vetor ordenado por slug para lookup;
- offsets em blob de conteúdo.

**Justificativa**

- transforma lookup de slug em `O(log n)` ou `O(1)`;
- desacopla apresentação da forma de armazenamento.

#### C. Separar parsing e rendering

Hoje `render_markdown()` parseia e já serializa. Para continuar sem heap:

- usar callbacks (`on_heading`, `on_paragraph`, `on_code`, `on_image`);
- o renderer decide se emite HTML, ops de DOM ou outro backend.

**Justificativa**

- melhora testabilidade;
- permite bench separado de parser;
- evita retrabalho em futuras saídas.

---

## Oportunidades específicas para WebAssembly

### 1. Redução de binary size

O contexto do build mostra `-Oz -flto`, o que é bom. Ainda assim há espaço adicional:

- remover exports não essenciais:
  - `_malloc` e `_free` só estão sendo exportados por causa de `hashchange`;
  - ao eliminar isso, o JS glue e a superfície exportada diminuem.
- remover código morto:
  - `match_str()`;
  - `embedded_posts[]`;
  - declarações públicas sem implementação.
- mover templates HTML/CSS longos para assets estáticos quando não precisarem estar no binário WASM.

**Justificativa**

String literals grandes dentro do módulo pesam no `.data`/`.rodata` e aumentam download e parse do módulo.

### 2. Layout de dados mais compacto

- `struct theme` com `uint8_t` ou `uint16_t`.
- `enum bar_seg_style` pode virar `uint8_t`.
- Se os gráficos crescerem, agrupar arrays paralelos em struct SoA/AoS conforme backend esperado.

**Justificativa**

No estado atual o ganho é pequeno, mas em WASM linear memory vale manter disciplina de packing cedo.

### 3. Menos crossing C <-> JS

- Hoje cada render termina em um único crossing grande, o que é melhor do que muitos crossings pequenos.
- Porém esse crossing passa uma string HTML inteira, que o JS reinterpreta.

**Recomendação**

- Se mantiver string: ao menos evitar crossings adicionais de imagens/metadados dentro do pipeline.
- Se migrar para command stream: enviar um buffer linear de comandos e payloads.

**Justificativa**

Crossing é frequentemente mais caro que trabalho local simples. A meta não deve ser só “menos crossings”, mas “crossings semanticamente densos”.

### 4. SIMD

Não há hotspot claro neste código que justifique SIMD imediato.

Possíveis candidatos futuros:

- escaping de HTML em bloco;
- scanner de delimitadores em Markdown;
- busca por `\n`, `$`, `]`.

**Avaliação**

No estado atual, SIMD não é prioridade. Antes disso, compensa:

- remover `innerHTML` full replace;
- corrigir scans bounded;
- medir com tracing real.

### 5. Alinhamento e acesso tipado no JS

`update_theme_colors()` lê a `struct theme` com offsets fixos do heap.

**Risco**

- Dependência forte de layout ABI.

**Recomendação**

- Exportar getters simples ou passar os seis índices como parâmetros escalares.

**Justificativa**

O ganho de “evitar alguns argumentos” não compensa a fragilidade do layout cruzando linguagens.

---

## Melhorias granulares recomendadas

### Segurança e corretude

1. Implementar escaping separado para:
   - texto HTML;
   - valor de atributo HTML;
   - string literal C no packer.

2. Tornar todos os scanners bounded por `end`.

3. Validar dimensões e percentuais de `[[graph:...]]`:
   - `0 <= pct <= 1`;
   - `0 <= opacity <= 1`;
   - `h > 0`, `w > 0`.

4. Trocar `isalnum`, `isspace`, `isdigit`, `isalpha` para uso com cast:
   - `isalnum((unsigned char)*src)`.

5. No packer, rejeitar slugs duplicados após sanitização.

   Justificativa: `hello-world.md` e `hello world.md` podem colidir.

### Performance

1. Remover redraw contínuo do canvas.
2. Substituir inline `onclick` por listener único no JS.
3. Trocar busca linear de slug por busca binária em vetor ordenado.
4. Se mantiver HTML string, reservar capacidade aproximada por página com base em metadados de tamanho do post.

### Escalabilidade

1. Introduzir camada `content_index` gerada em build com:
   - offset;
   - size;
   - slug;
   - title/date/description já escapados para a saída escolhida.

2. Separar o packer em:
   - leitura/validação;
   - normalização;
   - geração de código.

3. Adicionar testes golden para:
   - front matter;
   - fences;
   - inline math;
   - malformed graph shortcode;
   - strings com aspas e caracteres especiais.

---

## Ordem de implementação recomendada

### Fase 1: robustez imediata

- corrigir escaping em `src/ui.c` e `tools/packer.c`;
- corrigir scans bounded em `src/markdown.c`;
- corrigir slug bounded no packer;
- propagar erro de overflow do buffer.

### Fase 2: custo estrutural

- remover redraw contínuo;
- remover `_malloc/_free` do caminho de roteamento;
- acertar estado 404 e limpar APIs mortas/duplicadas.

### Fase 3: evolução arquitetural

- gerar índice por slug;
- migrar de HTML string para stream de comandos ou DOM builder seguro;
- separar parser Markdown de renderer.

---

## Conclusão

O projeto já tem uma base coerente para um engine minimalista compilado para WASM: código pequeno, sem heap no caminho quente e conteúdo resolvido em build-time. O problema é que a implementação atual concentra risco justamente nas fronteiras mais sensíveis: geração de HTML, parser bounded e empacotamento de conteúdo.

Se eu tivesse de resumir o estado em uma frase: a direção arquitetural é boa, mas a disciplina de contratos de dados ainda não está no mesmo nível da proposta do projeto.

O maior ganho prático virá de três mudanças:

1. tratar conteúdo como dado estruturado, não como HTML interpolado;
2. tornar todo parsing explicitamente bounded por `(ptr, len)`/`(cur, end)`;
3. reduzir o bridge WASM/JS para operações sem allocator e sem `innerHTML` full replace.

Isso melhora segurança, previsibilidade de memória, latência percebida e capacidade de crescimento sem abandonar a filosofia minimalista do engine.
