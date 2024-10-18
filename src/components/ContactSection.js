import React from 'react';

const ContactSection = () => (
  <section className="section" id="contact">
    <div className="container grid">
      <div className="text">
        <h2 className="title">Entre em contato!</h2>
        <p>
          Venha falar comigo e saber um pouco mais, expandir a networking e até mesmo tomar um café!
        </p>
        <a href="https://api.whatsapp.com/send?phone=+5511937071628&text=Oi! Gostaria de conversar com você sobre o livro de Kind, me fale mais um pouco sobre a linguagem" className="button" target="_blank">
          <i className="icon-whatsapp"></i>Me chama no zap
        </a>
      </div>
      <div className="links">
        <ul className="grid">
          <li>
            <i className="icon-phone"></i>
            (11) 93707-1628
          </li>
          <li>
            <i className="icon-mail"></i>
            bonatto@tutanota.com
          </li>
        </ul>
      </div>
    </div>
  </section>
);

export default ContactSection;
