let paralax = () => {
  // defino os elementos que movimentarei
  let stars = document.getElementById('stars')
  let moon = document.getElementById('moon')
  let mountains_behind = document.getElementById('mountains_behind')
  let mountains_between = document.getElementById('mountains_between')
  let mountains_front = document.getElementById('mountains_front')
  let text = document.getElementById('text')
  let catedralImg = document.getElementById('catedral');


  //ao movimentar o scroll, cada elemento se moverá uma distancia
  window.addEventListener('scroll', () => {
    let value = window.scrollY
    stars.style.left = `${value * 0.25}px`
    stars.style.top = A`${-value * 0.25}px`
    moon.style.top = `${value * 1.05}px`
    mountains_behind.style.top = `${value * 0.5}px`
    mountains_between.style.top = `${value * 0.75}px`
    mountains_front.style.top = `${value * 1}px`
    text.style.marginLeft = `${value * 3.5}px`
    text.style.marginTop = `${value * 0.75}px`
  })
}



// Objeto contendo os estilos para cada modo
const estilosModo = {
  light: {
    srcSatelite: './Imagens/parallax/sol.png',
    srcEstrela: './Imagens/parallax/nuvens.png',
    sateliteZIndex: '0',
    estrelaZIndex: '1',
    backgroundColor: '#2fb3eb'
  },
  dark: {
    srcSatelite: './Imagens/parallax/lua.png',
    srcEstrela: null,
    sateliteZIndex: '1',
    estrelaZIndex: '0',
    backgroundColor: '#1E1E1E'
  }
};

// Função para definir o modo de acordo com o horário ou com a escolha do usuário
function definirModo(modo = null) {
  let now = new Date();
  let hora = now.getHours();
  let oi = document.getElementById('text');
  let saudacao;

  // Definir saudação com base no horário
  if (hora >= 5 && hora < 12) {
    saudacao = 'Bom dia!';
  } else if (hora >= 12 && hora < 18) {
    saudacao = 'Boa tarde!';
  } else {
    saudacao = 'Boa noite!';
  }


  // Função para definir o modo de acordo com o horário ou com a escolha do usuário
function definirModo(modo = null) {
  let now = new Date();
  let hora = now.getHours();
  let oi = document.getElementById('text');
  let saudacao;

  // Definir saudação com base no horário
  if (hora >= 5 && hora < 12) {
    saudacao = 'Bom dia!';
  } else if (hora >= 12 && hora < 18) {
    saudacao = 'Boa tarde!';
  } else {
    saudacao = 'Boa noite!';
  }

  // Função para definir os estilos com base no modo
  function definirEstilo(estilo) {
    if (estilo.srcSatelite) {
      satelite.src = estilo.srcSatelite;
      satelite.style.height = '100px';
      satelite.style.marginTop = '100px';
      satelite.style.zIndex = estilo.sateliteZIndex;
    }
    if (estilo.srcEstrela) {
      estrela.src = estilo.srcEstrela;
      estrela.style.height = '90%';
      estrela.style.zIndex = estilo.estrelaZIndex;
    }
    if (estilo.backgroundColor) {
      document.body.style.background = estilo.backgroundColor;
    }
  }

  // Definir modo com base no horário se modo não for especificado
  if (modo === null) {
    if (hora >= 5 && hora < 12) {
      definirEstilo(estilosModo.light); // Modo light
      catedralImg.src = './Imagens/Catedral-dia.svg';
    } else if (hora >= 12 && hora < 18) {
      definirEstilo(estilosModo.light); // Modo light
      catedralImg.src = './Imagens/Catedral-dia.svg';
    } else {
      definirEstilo(estilosModo.dark); // Modo dark
      catedralImg.src = './Imagens/Catedral-noite.svg';
    }
  } else {
    // Definir modo com base na escolha do usuário
    definirEstilo(estilosModo[modo]);
    if (modo === 'light') {
      catedralImg.src = './Imagens/Catedral-dia.svg';
    } else {
      catedralImg.src = './Imagens/Catedral-noite.svg';
    }
  }

  // Definir a saudação
  oi.innerHTML = saudacao;
  paralax();
}

definirModo(); // Define o modo com base no horário
paralax(); // Movimentação paralax

definirModo(); // Define o modo com base no horário
paralax() // Movimentação paralax

//movimentação paralax
// paralax()

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
