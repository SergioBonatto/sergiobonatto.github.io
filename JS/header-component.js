// header-component.js
class HeaderComponent extends HTMLElement {
    constructor() {
      super();
    }

    connectedCallback() {
      this.innerHTML = `
        <header id="header">
          <nav class="container">
            <a class="logo" href="#"><span>Bonatto</span></a>
            <div class="menu">
              <ul class="grid">
                <li><a class="title" href="https://sergiobonatto.github.io/#home">Início</a></li>
                <li><a class="title" href="https://sergiobonatto.github.io/#about">Sobre</a></li>
                <li><a class="title" href="https://sergiobonatto.github.io/h#contact">Contato</a></li>
                <li><a class="title" href="https://sergiobonatto.github.io/blog.html">Blog</a></li>
                <li><a class="title" href="https://naoehsavio.github.io/learning/docs/Kind/index.html" target="_blank">Kind Book</a></li>
              </ul>
            </div>
            <div class="toggle icon-menu"></div>
            <div class="toggle icon-close"></div>
          </nav>
        </header>
      `;

      this.setupEventListeners();
    }

    setupEventListeners() {
      this.toggleMenu();
      this.esconderMenuAoClicar();
      window.addEventListener('scroll', () => {
        this.changeHeaderWhenScroll();
        this.activateMenuAtCurrentSection();
      });
    }

    toggleMenu() {
      const nav = this.querySelector('#header nav');
      const toggle = this.querySelectorAll('nav .toggle');

      for (const element of toggle) {
        element.addEventListener('click', () => {
          nav.classList.toggle('show');
        });
      }
    }

    esconderMenuAoClicar() {
      const links = this.querySelectorAll('nav ul li a');

      for (const link of links) {
        link.addEventListener('click', () => {
          this.querySelector('#header nav').classList.remove('show');
        });
      }
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

      for (let section of sections) {
        const sectionTop = section.offsetTop;
        const sectionHeight = section.offsetHeight;
        const sectionId = section.getAttribute('id');

        const checkpointStart = checkpoint >= sectionTop;
        const checkpointEnd = checkpoint <= sectionTop + sectionHeight;

        if (checkpointStart && checkpointEnd) {
          this.querySelector(`nav ul li a[href*=${sectionId}]`)?.classList.add('active');
        } else {
          this.querySelector(`nav ul li a[href*=${sectionId}]`)?.classList.remove('active');
        }
      }
    }
  }

  customElements.define('header-component', HeaderComponent);
