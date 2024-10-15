// parallax-component.js
class ParallaxSection extends HTMLElement {
    constructor() {
      super();
    }

    connectedCallback() {
      this.innerHTML = `
        <section class="section" id="home">
          <img src="./Imagens/parallax/ceu-estrelado.png" alt="nuvens" id="stars" />
          <img src="./Imagens/parallax/lua.png" alt="lua" id="moon" />
          <img src="./Imagens/parallax/morro_fundo.png" alt="morro ao fundo" id="mountains_behind" style="z-index: 7;" />
          <img src="./Imagens/parallax/bandeira.png" alt="bandeira" id="bandeira" style="z-index: 8;" />
          <img src="./Imagens/parallax/morro_frente.png" alt="morro ao fundo" id="mountains_front" style="z-index: 9;" />
          <h2 id="text" style="z-index: 9;">Olá tudo bem</h2>
          <img src="./Imagens/parallax/camara.png" alt="congresso" id="camara" style="z-index: 10;" />
          <img src="./Imagens/parallax/grama.png" alt="grama" id="grama" style="z-index: 11" />
        </section>
      `;

      // Chama as funções necessárias após a criação do componente
      this.definirSaudacao();
      this.setupParallax();
    }

    definirSaudacao() {
      // Implementação da função definirSaudacao
      const SAUDACOES = {
        MANHA: 'Bom dia!',
        TARDE: 'Boa tarde!',
        NOITE: 'Boa noite!'
      };

      const IMAGENS = {
        SOL: './Imagens/parallax/sol.png',
        NUVENS: './Imagens/parallax/nuvens.png'
      };

      const now = new Date();
      const hora = now.getHours();
      const oi = this.querySelector('#text');
      const satelite = this.querySelector('#moon');
      const estrela = this.querySelector('#stars');

      const configurarPeriodo = (saudacao, isDia) => {
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
      };

      if (hora >= 5 && hora < 12) {
        configurarPeriodo(SAUDACOES.MANHA, true);
      } else if (hora >= 12 && hora < 18) {
        configurarPeriodo(SAUDACOES.TARDE, true);
      } else {
        configurarPeriodo(SAUDACOES.NOITE, false);
      }
    }

    setupParallax() {
      const elements = {
        stars: this.querySelector('#stars'),
        moon: this.querySelector('#moon'),
        bandeira: this.querySelector('#bandeira'),
        camara: this.querySelector('#camara'),
        mountains_front: this.querySelector('#mountains_front'),
        mountains_behind: this.querySelector('#mountains_behind'),
        grama: this.querySelector('#grama'),
        text: this.querySelector('#text')
      };

      const maxScroll = window.innerHeight;

      const handleScroll = () => {
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
      };

      window.addEventListener('scroll', handleScroll);
    }
  }

  customElements.define('parallax-section', ParallaxSection);
