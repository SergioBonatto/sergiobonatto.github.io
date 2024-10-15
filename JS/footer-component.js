class Footer extends HTMLElement {
  constructor() {
    super();
  }

  connectedCallback() {
    this.innerHTML = `
      <footer class="section">
        <div class="container grid">
          <div class="brand">
            <a href="#home" class="logo logo-alt"><span>Bonatto</span></a>
            <p>Bonatto&trade; 2024</p>
            <p>Todos os direitos reservados.</p>
          </div>
          <div class="social grid">
            <a href="https://www.instagram.com/Sergio_Bonatto/" target="_blank">
              <i class="icon-instagram"></i>
            </a>
            <a href="https://github.com/SergioBonatto/" target="_blank">
              <img src="assets/fonts/github.svg" alt="GitHub" />
            </a>
            <a href="https://twitter.com/fiBonatto" target="_blank">
              <img src="assets/fonts/twitter.svg" alt="Twitter" />
            </a>
            <a href="https://www.linkedin.com/in/sergiobonatto/" target="_blank">
              <img src="assets/fonts/linkedin.svg" alt="LinkedIn" />
            </a>
          </div>
        </div>
      </footer>
    `;
  }
}

customElements.define('footer-component', Footer);
