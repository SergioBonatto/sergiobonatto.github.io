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
            <p>All rights reserved.</p>
          </div>
          <div class="social grid">
            <a href="https://www.instagram.com/Sergio_Bonatto/" target="_blank">
              Instagram
            </a>
            <a href="https://github.com/SergioBonatto/" target="_blank">
              Github
            </a>
            <a href="https://twitter.com/fiBonatto" target="_blank">
              X
            </a>
            <a href="https://www.linkedin.com/in/sergiobonatto/" target="_blank">
              Linkedin
            </a>
          </div>
        </div>
      </footer>
    `;
  }
}

customElements.define('footer-component', Footer);
