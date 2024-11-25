class HeaderComponent extends HTMLElement {
  constructor() {
    super();
    // Inicializa estados
    this.isMenuOpen = false;
  }

  connectedCallback() {
    this.innerHTML = `
      <header id="header">
        <nav class="container">
          <a class="logo" href="#"><span>Bonatto</span></a>
          <div class="menu-buttons">
            <button class="toggle icon-menu" aria-label="Open Menu">☰</button>
            <button class="toggle icon-close" aria-label="Close Menu">✖</button>
          </div>
          <div class="menu">
            <ul class="grid">
              <li><a class="title" href="https://sergiobonatto.github.io/#home">Home</a></li>
              <li><a class="title" href="https://sergiobonatto.github.io/#about">About</a></li>
              <li><a class="title" href="https://sergiobonatto.github.io/#contact">Contact</a></li>
              <li><a class="title" href="https://sergiobonatto.github.io/blog.html">Blog</a></li>
              <li><a class="title" href="https://naoehsavio.github.io/learning/docs/Kind/index.html" target="_blank">Kind Book</a></li>
            </ul>
          </div>
        </nav>
      </header>
    `;

    this.setupEventListeners();
  }

  setupEventListeners() {
    // Inicializa todos os event listeners quando o componente é montado
    this.toggleMenu();
    this.hideMenuOnClick();
    this.setupScrollListeners();
  }

  toggleMenu() {
    const nav = this.querySelector('nav');
    const menuButton = this.querySelector('.icon-menu');
    const closeButton = this.querySelector('.icon-close');
    const menu = this.querySelector('.menu');

    const toggleMenuState = () => {
      this.isMenuOpen = !this.isMenuOpen;
      nav.classList.toggle('show', this.isMenuOpen);

      // Gerencia acessibilidade e visibilidade dos botões
      menuButton.style.display = this.isMenuOpen ? 'none' : 'block';
      closeButton.style.display = this.isMenuOpen ? 'block' : 'none';

      // Previne scroll quando menu está aberto
      document.body.style.overflow = this.isMenuOpen ? 'hidden' : '';
    };

    // Configura os event listeners
    menuButton.addEventListener('click', toggleMenuState);
    closeButton.addEventListener('click', toggleMenuState);

    // Inicializa o estado do menu
    closeButton.style.display = 'none';
  }

  hideMenuOnClick() {
    const nav = this.querySelector('nav');
    const menuButton = this.querySelector('.icon-menu');
    const closeButton = this.querySelector('.icon-close');
    const menu = this.querySelector('.menu');
    const links = this.querySelectorAll('nav ul li a');

    links.forEach(link => {
      link.addEventListener('click', () => {
        this.isMenuOpen = false;
        nav.classList.remove('show');
        menu.style.display = 'none';
        menuButton.style.display = 'block';
        closeButton.style.display = 'none';
        document.body.style.overflow = '';
      });
    });
  }

  setupScrollListeners() {
    let lastScroll = 0;
    const header = this.querySelector('#header');

    window.addEventListener('scroll', () => {
      // Gerencia mudanças no header durante scroll
      this.changeHeaderWhenScroll();
      this.activateMenuAtCurrentSection();

      // Adiciona comportamento de esconder/mostrar header baseado na direção do scroll
      const currentScroll = window.pageYOffset;
      if (currentScroll > lastScroll && currentScroll > 100) {
        header.style.transform = 'translateY(-100%)';
      } else {
        header.style.transform = 'translateY(0)';
      }
      lastScroll = currentScroll;
    });
  }

  changeHeaderWhenScroll() {
    const header = this.querySelector('#header');
    const navHeight = header.offsetHeight;

    if (window.scrollY >= navHeight) {
      header.classList.add('scroll');
    } else {
      header.classList.remove('scroll');
    }
  }

  activateMenuAtCurrentSection() {
    const sections = document.querySelectorAll('main section[id]');
    const checkpoint = window.pageYOffset + (window.innerHeight / 8) * 4;

    sections.forEach(section => {
      const sectionTop = section.offsetTop;
      const sectionHeight = section.offsetHeight;
      const sectionId = section.getAttribute('id');
      const menuLink = this.querySelector(`nav ul li a[href*=${sectionId}]`);

      const checkpointStart = checkpoint >= sectionTop;
      const checkpointEnd = checkpoint <= sectionTop + sectionHeight;

      if (checkpointStart && checkpointEnd && menuLink) {
        // Remove active de todos os links antes de adicionar ao atual
        this.querySelectorAll('nav ul li a').forEach(link => {
          link.classList.remove('active');
        });
        menuLink.classList.add('active');
      }
    });
  }
}

customElements.define('header-component', HeaderComponent);
