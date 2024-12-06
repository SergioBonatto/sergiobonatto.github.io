document.addEventListener('DOMContentLoaded', () => {
  // Função para exibir/ocultar o botão de voltar ao topo
  function backToTop() {
    const backToTopButton = document.querySelector('.back-to-top');
    if (window.scrollY >= 560) {
      backToTopButton.classList.add('show');
    } else {
      backToTopButton.classList.remove('show');
    }
  }

  // Adiciona evento de clique para rolar até a div .visitor-counter
  document.querySelector('.back-to-top').addEventListener('click', () => {
    const visitorCounter = document.querySelector('.visitor-counter');
    if (visitorCounter) {
      visitorCounter.scrollIntoView({ behavior: 'smooth' });
    } else {
      console.warn('Elemento .visitor-counter não encontrado!');
    }
  });

  // Inicialização das funções
  window.addEventListener('scroll', () => {
    backToTop();
  });
});

document.addEventListener('DOMContentLoaded', () => {
  const menuButton = document.querySelector('.icon-menu');
  const closeButton = document.querySelector('.icon-close');
  const menu = document.querySelector('.menu');

  menuButton.addEventListener('click', () => {
    menu.classList.add('show');
    menuButton.style.display = 'none';
    closeButton.style.display = 'block';
  });

  closeButton.addEventListener('click', () => {
    menu.classList.remove('show');
    menuButton.style.display = 'block';
    closeButton.style.display = 'none';
  });
});


document.addEventListener('DOMContentLoaded', () => {
  function getRandomNumber(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }

  function updateVisitorCounter() {
    const visitorCounter = document.querySelector('.visitor-counter');
    const randomNumber = getRandomNumber(68, 421);
    visitorCounter.textContent = `Visitors: ${String(randomNumber).padStart(7, '0')}`;
  }

  setInterval(updateVisitorCounter, 2500);
});
