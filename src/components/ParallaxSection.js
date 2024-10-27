// src/components/ParallaxSection.js
import React, { useEffect } from 'react';
import '../CSS/ParallaxSection.css'; // Importe o CSS correspondente

const ParallaxSection = () => {
  useEffect(() => {
    definirSaudacao();
    setupParallax();
  }, []);

  const definirSaudacao = () => {
    const SAUDACOES = {
      MANHA: 'Bom dia!',
      TARDE: 'Boa tarde!',
      NOITE: 'Boa noite!'
    };

    const IMAGENS = {
      SOL: '/Imagens/parallax/sol.png',
      NUVENS: '/Imagens/parallax/nuvens.png'
    };

    const now = new Date();
    const hora = now.getHours();
    const oi = document.querySelector('#text');
    const satelite = document.querySelector('#moon');
    const estrela = document.querySelector('#stars');

    const configurarPeriodo = (saudacao, isDia) => {
      oi.textContent = saudacao;
      if (isDia) {
        satelite.src = IMAGENS.SOL;
        satelite.style.height = '100px';
        satelite.style.marginTop = '100px';
        satelite.style.zIndex = '0';
        document.body.style.background = '#0098ec';
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
  };

  const setupParallax = () => {
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

    return () => {
      window.removeEventListener('scroll', handleScroll);
    };
  };

  return (
    <section className="section" id="home">
      <img src="/Imagens/parallax/ceu-estrelado.png" alt="nuvens" id="stars" />
      <img src="/Imagens/parallax/lua.png" alt="lua" id="moon" />
      <img src="/Imagens/parallax/morro_fundo.png" alt="morro ao fundo" id="mountains_behind" style={{ zIndex: 7 }} />
      <img src="/Imagens/parallax/bandeira.png" alt="bandeira" id="bandeira" style={{ zIndex: 8 }} />
      <img src="/Imagens/parallax/morro_frente.png" alt="morro ao fundo" id="mountains_front" style={{ zIndex: 9 }} />
      <h2 id="text" style={{ zIndex: 9 }}>Olá tudo bem</h2>
      <img src="/Imagens/parallax/camara.png" alt="congresso" id="camara" style={{ zIndex: 10 }} />
      <img src="/Imagens/parallax/grama.png" alt="grama" id="grama" style={{ zIndex: 11 }} />
    </section>
  );
};

export default ParallaxSection;
