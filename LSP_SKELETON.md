# Skeleton para Automação de LSP (C/Cross-Compilation)

Este documento fornece a estrutura para automatizar a configuração do `clangd` em novos projetos, garantindo que o LSP funcione perfeitamente no macOS, seja para C nativo ou Emscripten/WASM.

## 1. O Template Universal (`.clangd`)

O arquivo `.clangd` é superior ao `compile_commands.json` para resolver problemas de ambiente porque ele permite **filtrar** e **injetar** flags antes que o LSP as processe.

### Estrutura Recomendada
```yaml
CompileFlags:
  Add: [
    "-Iinclude",
    "-Isrc",
    "-Wall",
    "-std=c11",
    # macOS SDK (substituir pelo caminho do xcrun --show-sdk-path)
    "-isysroot", "/Applications/Xcode.app/.../MacOSX.sdk"
  ]
  Remove: [
    "-mllvm",           # Quebra o clangd
    "-fignore-exceptions",
    "--sysroot=*"       # Remove sysroots conflitantes de builds externos
  ]
Index:
  Background: true
```

## 2. Script de Automação (`setup_lsp.sh`)

Crie este script na sua pasta de "templates" para rodar em novos projetos.

```bash
#!/bin/bash
# setup_lsp.sh

# Detecta o SDK do macOS dinamicamente
SDK_PATH=$(xcrun --show-sdk-path)

echo "CompileFlags:" > .clangd
echo "  Add: [" >> .clangd
echo "    \"-Iinclude\"," >> .clangd
echo "    \"-isysroot\", \"$SDK_PATH\"," >> .clangd

# Se o projeto for Emscripten, injeta os headers do WASM
if [ -f "build.sh" ] && grep -q "emcc" "build.sh"; then
    echo "    \"-DEMSCRIPTEN\"," >> .clangd
    echo "    \"--target=wasm32-unknown-emscripten\"," >> .clangd
    
    # Pega os includes do Emscripten via pré-processador
    EM_INCLUDES=$(emcc -xc -E -v - < /dev/null 2>&1 | grep "^ /" | sed 's/^ //')
    for path in $EM_INCLUDES; do
        echo "    \"-isystem\", \"$path\"," >> .clangd
    done
fi

echo "    \"-std=c11\"" >> .clangd
echo "  ]" >> .clangd
echo "  Remove: [\"-mllvm\", \"--sysroot=*\"]" >> .clangd

echo "LSP configurado em .clangd"
```

## 3. Por que isso funciona?
1.  **Dinamismo:** O script usa `xcrun` e `emcc` para descobrir onde os headers estão na **sua** máquina hoje.
2.  **Isolamento:** O uso de `-isystem` para os headers do Emscripten garante que o LSP os trate como arquivos de sistema, evitando que o linter aponte erros dentro da biblioteca padrão.
3.  **Target Awareness:** Ao definir `--target`, o `clangd` entende o tamanho dos tipos (`size_t`, etc) de acordo com a plataforma alvo (WASM de 32 bits vs Mac de 64 bits).

## 4. Integração com Build System
Sempre que o seu `build.sh` gerar um `compile_commands.json`, o `.clangd` atuará como uma camada de correção, garantindo que, mesmo que o JSON aponte para caminhos de build temporários, o seu editor sempre encontre os headers reais.
