// src/components/FooterComponent.js
import React from 'react';
import '../CSS/footer.css';
import '../CSS/base.css';

const FooterComponent = () => (
  <footer className="section">
    <div className="container grid">
      <div className="brand">
        <a href="#home" className="logo logo-alt"><span>Bonatto</span></a>
        <p>Bonatto&trade; 2024</p>
        <p>Todos os direitos reservados.</p>
      </div>
      <div className="social grid">
        <a href="https://www.instagram.com/Sergio_Bonatto/" target="_blank" rel="noopener noreferrer">
        <img src="/fonts/instagram.svg" alt="GitHub" />
        </a>
        <a href="https://github.com/SergioBonatto/" target="_blank" rel="noopener noreferrer">
          <img src="/fonts/github.svg" alt="GitHub" />
        </a>
        <a href="https://twitter.com/fiBonatto" target="_blank" rel="noopener noreferrer">
          <img src="/fonts/twitter.svg" alt="Twitter" className="feather feather-twitter" />
        </a>
        <a href="https://www.linkedin.com/in/sergiobonatto/" target="_blank" rel="noopener noreferrer">
          <img src="/fonts/linkedin.svg" alt="LinkedIn" />
        </a>
      </div>
    </div>
  </footer>
);

export default FooterComponent
