# Seu terminal pode ser muito mais produtivo, bonito e amigável!

## Que tal deixar pra lá aquele terminal sem graça e "burro", que você precisa fazer tudo "na mão"?

Aquele velho terminal em preto e verde, lembrando um monitor de fósforo verde já passou, ficou para o passado e, junto dele, a necessidade de fazer cada coisa repetidamente. Afinal, não é para isso que trabalhamos, automatizar tarefaz repetitivas?

As seguintes dicas servem para basicamente todos os terminais e sistemas operacionais, mas eu recomendo fortemente o uso de um Linux (ou ao menos um MacOS) e o terminal Kitty, não posso negar, ele é o meu favorito.

Primeiro, para o Ubuntu:

```bash
sudo add-apt-repository universe
sudo apt update
sudo apt install kitty
```

Debian:

```bash
sudo apt-get install kitty
```

Arch Linux (vulgo, melhor distro do mundo):

```bash
sudo pacman -S kitty
```

Fedora:

```bash
sudo dnf install kitty
```

OpenSUSE:

```bash
sudo zypper install kitty
```

Após instalar o Kitty, nos falta customizar, ele vem "bem cru", o que é bom no geral, pois é mais simples deixar ele com a nossa cara.

Primeiro, devemos preparar a customização, para isso crie uma pasta e o arquivo para guardar as configurações (caso já não tenham sidos criados):

```bash
mkdir -p ~/.config/kitty/
touch ~/.config/kitty/kitty.conf
```

Após isso clone o repositório do Gruvbox-material (é o que eu recomendo, é o mais agradável para longos períodos de programação)

```bash
cd ~/.config/kitty
```

E então clone este [repositório](https://github.com/rsaihe/gruvbox-material-kitty)

Dentro da pasta do gruvbox há algumas variações do tema, desde os mais claros aos mais escuros, escolha o seu e inclua no final do arquivo kitty.conf

```bash
include "CAMINHO PARA O ARQUIVO"
```

Para deixar seu terminal ainda mais poderoso, recomendo o uso da fonte [Fira Code](https://github.com/tonsky/FiraCode/wiki/Installing)

Feito isso, falta apenas usar o Oh-my-zsh e para isso, devemos instalar o ZSH

Fedora:

```bash
dnf install zsh
```

Arch:

```bash
pacman -S zsh zsh-completions
```

OpenSUSE:

```bash
zypper in zsh
```

Ubuntu:

```bash
apt install zsh
```

OpenBSD:

```bash
pkg install zsh bash
```

Feito isso, o Oh-my-zsh pré-configurará o restante do nosso terminal:

```bash
curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh | sh; zsh
```

É possível que, após a instalação, seu terminal ainda não tenha alterado o shell padrão, para isso basta digitar o seguinte comando:

```bash
sudo usermod --shell $(which zsh) $USER
```

Pronto, a parte mais chata já foi feita, basta terminar instalando os plugins desejados, dois que uso são o zsh-syntax-highlighting e o zsh-autosuggestions, para instalar devemos dar os seguintes comandos:

```bash
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
```

e

```bash
git clone https://github.com/zsh-users/zsh-autosuggestions $ZSH_CUSTOM/plugins/zsh-autosuggestions
```

Para arrematar, falta apenas adicionar na lista dos plugins ativos no seu "~/.zshrc", eis a lista dos que eu uso:

```bash
plugins=(
git
zsh-syntax-highlighting
zsh-autosuggestions
catimg
copybuffer
copyfile
dircycle
dirhistory
extract
git-prompt
gitfast
gitignore
history
jsontools
fzf
)
```

Caso deseje usar os mesmos plugins, o Fzf (um buscador de arquivos) deve ser instalado de igual forma aos outros dois, basta dar o seguinte comando e responder "y" para as questões que serão feitas:

```bash
git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf && ~/.fzf/install
```

Dica bônus, um tema mais limpo e legal:

```bash
git clone https://github.com/cdimascio/lambda-zsh-theme.git && cp cdimascio-lambda.zsh-theme $ZSH_CUSTOM/themes
```

Depois altere a linha que começa com ZSH_THEME para:

```bash
ZSH_THEME="cdimascio-lambda"
```

e por fim rode "source ~/.zshrc" para ativar as configurações.

Caso tenha seguido os passos necessários, seu terminal estará assim:

![meu terminal](./Imagens/my_terminal.png)
