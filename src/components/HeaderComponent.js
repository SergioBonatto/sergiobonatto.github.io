// src/components/HeaderComponent.js
import React, { useEffect } from 'react';
import '../CSS/HeaderComponent.css'; // Importe o CSS correspondente

const HeaderComponent = () => {
  useEffect(() => {
    const toggleMenu = () => {
      const nav = document.querySelector('.header nav');
      const toggle = document.querySelectorAll('nav .toggle');

      for (const element of toggle) {
        element.addEventListener('click', () => {
          nav.classList.toggle('show');
        });
      }
    };

    const esconderMenuAoClicar = () => {
      const links = document.querySelectorAll('nav ul li a');

      for (const link of links) {
        link.addEventListener('click', () => {
          document.querySelector('.header nav').classList.remove('show');
        });
      }
    };

    const changeHeaderWhenScroll = () => {
      const header = document.querySelector('.header');
      const navHeight = header.offsetHeight;

      if (window.scrollY >= navHeight) {
        header.classList.add('scroll');
      } else {
        header.classList.remove('scroll');
      }
    };

    const activateMenuAtCurrentSection = () => {
      const sections = document.querySelectorAll('main section[id]');
      const checkpoint = window.pageYOffset + (window.innerHeight / 8) * 4;

      for (let section of sections) {
        const sectionTop = section.offsetTop;
        const sectionHeight = section.offsetHeight;
        const sectionId = section.getAttribute('id');

        const checkpointStart = checkpoint >= sectionTop;
        const checkpointEnd = checkpoint <= sectionTop + sectionHeight;

        if (checkpointStart && checkpointEnd) {
          document.querySelector(`nav ul li a[href*=${sectionId}]`)?.classList.add('active');
        } else {
          document.querySelector(`nav ul li a[href*=${sectionId}]`)?.classList.remove('active');
        }
      }
    };

    toggleMenu();
    esconderMenuAoClicar();
    window.addEventListener('scroll', () => {
      changeHeaderWhenScroll();
      activateMenuAtCurrentSection();
    });

    return () => {
      window.removeEventListener('scroll', changeHeaderWhenScroll);
      window.removeEventListener('scroll', activateMenuAtCurrentSection);
    };
  }, []);

  return (
    <header className="header">
      <nav className="container">
        <a className="logo" href="#"><span>Bonatto</span></a>
        <div className="menu">
        <ul class="grid">
                <li><a class="title" href="https://sergiobonatto.github.io/#home">Início</a></li>
                <li><a class="title" href="https://sergiobonatto.github.io/#about">Sobre</a></li>
                <li><a class="title" href="https://sergiobonatto.github.io/h#contact">Contato</a></li>
                <li><a class="title" href="https://sergiobonatto.github.io/blog.html">Blog</a></li>
                <li><a class="title" href="https://naoehsavio.github.io/learning/docs/Kind/index.html" target="_blank">Kind Book</a></li>
              </ul>
        </div>
        <div className="toggle icon-menu"></div>
        <div className="toggle icon-close"></div>
      </nav>
    </header>
  );
};

export default HeaderComponent;
