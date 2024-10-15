// saudação por horário
// capturo o horario do dispositivo
let now = new Date()
let hora = now.getHours()
// Constantes para strings repetidas
const SAUDACOES = {
	MANHA: 'Bom dia!',
	TARDE: 'Boa tarde!',
	NOITE: 'Boa noite!'
  };

  const IMAGENS = {
	SOL: './Imagens/parallax/sol.png',
	NUVENS: './Imagens/parallax/nuvens.png'
  };

  // Função para definir a saudação e alterar elementos conforme o horário
  const definirSaudacao = () => {
	const now = new Date();
	const hora = now.getHours();
	const oi = document.querySelector('#text');
	const satelite = document.querySelector('#moon');
	const estrela = document.querySelector('#stars');

	if (hora >= 5 && hora < 12) {
	  configurarPeriodo(SAUDACOES.MANHA, true);
	} else if (hora >= 12 && hora < 18) {
	  configurarPeriodo(SAUDACOES.TARDE, true);
	} else {
	  configurarPeriodo(SAUDACOES.NOITE, false);
	}

	function configurarPeriodo(saudacao, isDia) {
	  oi.textContent = saudacao;
	  if (isDia) {
		satelite.src = IMAGENS.SOL;
		satelite.style.height = '100px';
		satelite.style.marginTop = '100px';
		satelite.style.zIndex = '0';
		document.body.style.background = '#2fb3eb';
		estrela.src = IMAGENS.NUVENS;
		estrela.style.height = '90%';
		estrela.style.zIndex = '1';
	  } else {
		satelite.style.zIndex = '1';
		estrela.style.zIndex = '0';
	  }
	}
  };

  const parallax = () => {
    const elements = {
      stars: document.querySelector('#stars'),
      moon: document.querySelector('#moon'),
      bandeira: document.querySelector('#bandeira'),
      camara: document.querySelector('#camara'),
      mountains_front: document.querySelector('#mountains_front'),
      mountains_behind: document.querySelector('#mountains_behind'),
      grama: document.querySelector('#grama'),
      text: document.querySelector('#text')
    };

    const maxScroll = window.innerHeight;

    window.addEventListener('scroll', () => {
      const value = Math.min(window.scrollY, maxScroll);
      const scrollFactor = value / maxScroll;

      elements.stars.style.left = `${value * 0.25}px`;
      elements.stars.style.top = `${-value * 0.25}px`;
      elements.moon.style.top = `${value * 1.05}px`;
      elements.camara.style.top = `${value * 0.5}px`;
      elements.camara.style.marginLeft = `${value * 0.09}px`;
      elements.mountains_behind.style.top = `${value * 1.75}px`;
      elements.mountains_front.style.top = `${value * 0.75}px`;
      elements.bandeira.style.top = `${value * 1.5}px`;
      elements.bandeira.style.marginLeft = `${-value * 0.09}px`;
      elements.text.style.marginLeft = `${value * 3.5}px`;
      elements.text.style.marginTop = `${value * 0.75}px`;

      // Fade out dos elementos de parallax após a altura da tela
      if (window.scrollY > maxScroll) {
        const fadeOutFactor = 1 - Math.min((window.scrollY - maxScroll) / (maxScroll / 2), 1);
        Object.values(elements).forEach(el => {
          if (el !== elements.text) {
            el.style.opacity = fadeOutFactor;
          }
        });
      } else {
        Object.values(elements).forEach(el => {
          if (el !== elements.text) {
            el.style.opacity = 1;
          }
        });
      }
    });
  };
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
