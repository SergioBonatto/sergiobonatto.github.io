// saudação por horário
// capturo o horario do dispositivo
let now = new Date()
let hora = now.getHours()
//defino os elementos que quero alterar
let oi = window.document.getElementById('text')
let satelite = window.document.getElementById('moon')
let estrela = window.document.getElementById('stars')

//se for entre 5 horas da manha e 12 horas
if (hora >= 5 && hora < 12) {
  // a mensagem é de bom dia
  oi.innerHTML = 'Bom dia!'
  //altero o  elemento satélite para o Sol
  satelite.src = './Imagens/parallax/sol.png'
  //defino o tamanho
  satelite.style.height = '100px'
  satelite.style.marginTop = '100px'
  // e a posição
  satelite.style.zIndex = '0'
  //mudo a cor de fundo para um azul similar ao céu de dia
  document.body.style.background = '#2fb3eb'
  // tiro as estrelas e coloco nuvens no lugar
  estrela.src = './Imagens/parallax/nuvens.png'
  //altero o tamanho das nuvens
  estrela.style.height = '90%'
  // e coloco na frente do sol
  estrela.style.zIndex = '1'
}
// se não, se for entre 12 horas e 18 da tarde
else if (hora >= 12 && hora < 18) {
  // mudo a mensagem para "Boa tarde"
  oi.innerHTML = 'Boa tarde!'
  // o resto permanece igual ao da manhã
  satelite.src = './Imagens/parallax/sol.png'
  satelite.style.height = '100px'
  satelite.style.marginTop = '100px'
  satelite.style.zIndex = '0'
  document.body.style.background = '#2fb3eb'
  estrela.src = './Imagens/parallax/nuvens.png'
  estrela.style.height = '90%'
  estrela.style.zIndex = '1'
}
// se não for manhã e nem tarde
else {
  // a mensagem vira "Boa noite"
  oi.innerHTML = 'Boa noite!'
  // a lua vem para a frente
  satelite.style.zIndex = '1'
  // e as estrelas vão para o fundo
  estrela.style.zIndex = '0'
}

//movimentação paralax
let paralax = () => {
  // defino os elementos que movimentarei
  let stars = document.getElementById('stars')
  let moon = document.getElementById('moon')
  let mountains_behind = document.getElementById('mountains_behind')
  let mountains_between = document.getElementById('mountains_between')
  let mountains_front = document.getElementById('mountains_front')
  let marco = document.getElementById('marco')
  let grama = document.getElementById('grama')
  let text = document.getElementById('text')

  //ao movimentar o scroll, cada elemento se moverá uma distancia
  window.addEventListener('scroll', () => {
    let value = window.scrollY
    stars.style.left = `${value * 0.25}px`
    stars.style.top = `${-value * 0.25}px`
    moon.style.top = `${value * 1.05}px`
    mountains_behind.style.top = `${value * 0.5}px`
    mountains_between.style.top = `${value * 0.75}px`
    mountains_front.style.top = `${value * 1}px`
    text.style.marginLeft = `${value * 3.5}px`
    text.style.marginTop = `${value * 0.75}px`
  })
}
paralax()

const nav = document.querySelector('#header nav')
const toggle = document.querySelectorAll('nav .toggle')

for (const element of toggle) {
  element.addEventListener('click', () => {
    nav.classList.toggle('show')
  })
}

// Quando clicar em um item, esconder o menu
const links = document.querySelectorAll('nav ul li a')

for (const link of links) {
  link.addEventListener('click', () => {
    nav.classList.remove('show')
  })
}

// Sombra no Header qdo for dado o scroll na pagina
const header = document.querySelector('#header')
const navHeight = header.offsetHeight

var changeHeaderWhenScroll = () => {
  if (window.scrollY >= navHeight) {
    // O scroll é maior que a altura do header
    header.classList.add('scroll')
  } else {
    // O scroll é menor que a altura do header
    header.classList.remove('scroll')
  }
}

//Back to top button, voltar par o topo
const backToTopButton = document.querySelector('.back-to-top')

var backToTop = () => {
  if (scrollY >= 360) {
    backToTopButton.classList.add('show')
  } else {
    backToTopButton.classList.remove('show')
  }
}

// Menu ativo conforme seção visível na página
const sections = document.querySelectorAll('main section[id]')

let activateMenuAtCurrentSection = () => {
  /* divide a alura da tela em 8, multiplica por 4 e soma com o deslocamento do eixo Y */
  const checkpoint = window.pageYOffset + (window.innerHeight / 8) * 4
  /* Para cada item na lista de sessões seção */
  for (let section of sections) {
    /* Ele cria uma variável */
    const sectionTop = section.offsetTop /* define o topo da seção */
    const sectionHeight = section.offsetHeight /* define a altura da seção */
    const sectionId =
      section.getAttribute('id') /* seleciona as IDs das seções */

    /* Cria o ponto de controle inicial da função */
    /* que deve ser o checkpoin desde que ele seja maior ou igual ao topo da seção */
    const checkpointStart = checkpoint >= sectionTop
    /* cria o ponto de controle final da função */
    /* que será quando o checkpoint for menor que o tamanho do topo mais a altura da seção, pegando a parte bem de baixo */
    const checkpointEnd = checkpoint <= sectionTop + sectionHeight
    /* Se o checkpoint estiver entre o checkpoint Start e o checkpointEnd */
    /* ou seja, com a seção visível */
    if (checkpointStart && checkpointEnd) {
      /* seleciona especificamente, na barra de navegação e dentro das listas, o link que tiver como referencia a id da seção visivel */
      document
        .querySelector(`nav ul li a[href*=${sectionId}]`)
        .classList.add('active')
    } else {
      document
        .querySelector(`nav ul li a[href*=${sectionId}]`)
        .classList.remove('active')
    }
  }
}

//When scroll
window.addEventListener('scroll', () => {
  changeHeaderWhenScroll()
  backToTop()
  activateMenuAtCurrentSection()
})
