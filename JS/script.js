function loadSection(sectionId, filePath) {
  fetch(filePath)
    .then(response => response.text())
    .then(data => {
      document.getElementById(sectionId).innerHTML = data;
    })
    .catch(error => console.error('Erro ao carregar a seção:', error));
}

// Carregar a seção home
loadSection('home-section-container', './home-section.html');

// Função para definir a saudação e alterar elementos conforme o horário
function definirSaudacao() {
  let now = new Date();
  let hora = now.getHours();
  let oi = document.getElementById('text');
  let satelite = document.getElementById('moon');
  let estrela = document.getElementById('stars');
  let bandeira = document.getElementById('bandeira');

  if (hora >= 5 && hora < 12) {
    configurarDia(oi, satelite, bandeira, estrela, 'Bom dia!');
  } else if (hora >= 12 && hora < 18) {
    configurarDia(oi, satelite, bandeira, estrela, 'Boa tarde!');
  } else {
    configurarNoite(oi, satelite, estrela);
  }
}

function configurarDia(oi, satelite, bandeira, estrela, saudacao) {
  oi.innerHTML = saudacao;
  satelite.src = './Imagens/parallax/sol.png';
  bandeira.src = './Imagens/parallax/bandeira_dia.png';
  satelite.style.height = '100px';
  satelite.style.marginTop = '100px';
  satelite.style.zIndex = '0';
  document.body.style.background = '#2fb3eb';
  estrela.src = './Imagens/parallax/nuvens.png';
  estrela.style.height = '90%';
  estrela.style.zIndex = '1';
}

function configurarNoite(oi, satelite, estrela) {
  oi.innerHTML = 'Boa noite!';
  satelite.style.zIndex = '1';
  estrela.style.zIndex = '0';
}

// Função para movimentação parallax
function parallax() {
  let stars = document.getElementById('stars');
  let moon = document.getElementById('moon');
  let bandeira = document.getElementById('bandeira');
  let camara = document.getElementById('camara');
  let mountains_front = document.getElementById('mountains_front');
  let text = document.getElementById('text');

  window.addEventListener('scroll', () => {
    let value = window.scrollY;
    stars.style.left = `${value * 0.25}px`;
    stars.style.top = `${-value * 0.25}px`;
    moon.style.top = `${value * 1.05}px`;
    bandeira.style.top = `${value / 2.5}px`;
    camara.style.top = `${value * 0.75}px`;
    mountains_front.style.top = `${value / 9.5}px`;
    text.style.marginLeft = `${value * 3.5}px`;
    text.style.marginTop = `${value * 0.75}px`;
  });
}

// Função para alternar o menu
function toggleMenu() {
  const nav = document.querySelector('#header nav');
  const toggle = document.querySelectorAll('nav .toggle');

  for (const element of toggle) {
    element.addEventListener('click', () => {
      nav.classList.toggle('show');
    });
  }
}

// Função para esconder o menu ao clicar em um item
function esconderMenuAoClicar() {
  const links = document.querySelectorAll('nav ul li a');

  for (const link of links) {
    link.addEventListener('click', () => {
      document.querySelector('#header nav').classList.remove('show');
    });
  }
}

// Função para adicionar sombra no header ao dar scroll
function changeHeaderWhenScroll() {
  const header = document.querySelector('#header');
  const navHeight = header.offsetHeight;

  if (window.scrollY >= navHeight) {
    header.classList.add('scroll');
  } else {
    header.classList.remove('scroll');
  }
}

// Função para mostrar o botão de voltar ao topo
function backToTop() {
  const backToTopButton = document.querySelector('.back-to-top');

  if (scrollY >= 360) {
    backToTopButton.classList.add('show');
  } else {
    backToTopButton.classList.remove('show');
  }
}

// Função para ativar o menu conforme a seção visível
function activateMenuAtCurrentSection() {
  const sections = document.querySelectorAll('main section[id]');
  const checkpoint = window.pageYOffset + (window.innerHeight / 8) * 4;

  for (let section of sections) {
    const sectionTop = section.offsetTop;
    const sectionHeight = section.offsetHeight;
    const sectionId = section.getAttribute('id');

    const checkpointStart = checkpoint >= sectionTop;
    const checkpointEnd = checkpoint <= sectionTop + sectionHeight;

    if (checkpointStart && checkpointEnd) {
      document
        .querySelector(`nav ul li a[href*=${sectionId}]`)
        .classList.add('active');
    } else {
      document
        .querySelector(`nav ul li a[href*=${sectionId}]`)
        .classList.remove('active');
    }
  }
}

// Inicialização das funções
definirSaudacao();
parallax();
toggleMenu();
esconderMenuAoClicar();
window.addEventListener('scroll', () => {
  changeHeaderWhenScroll();
  backToTop();
  activateMenuAtCurrentSection();
});
