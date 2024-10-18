import React from 'react';

const AboutSection = () => (
  <section className="section" id="about">
    <div className="container grid">
      <div className="image">
        <img src="./Imagens/perfil.png" alt="Foto minha com óculos escuros e nuvens ao fundo" />
      </div>
      <div className="text">
        <h2 className="title">Sobre mim</h2>
        <p>
          Eu sou um programador na 
          <a id="kindelia" href="https://higherorderco.com/"> Higher Order Company</a>,
          um entusiasta da programação funcional e das tecnologias descentralizadas, que garantem a privacidade & autonomia do usuário.
        </p>
        <br />
        <p>
          Sou pai do Matteo, teólogo reformado & autodidata, estou escrevendo um livro sobre Kind e provas formais, sou amante da filosofia de código aberto, nerd, lindo, vegano não-praticante e apaixonado por café.
        </p>
        <br />
        <p>
          Acesse meu <a id="kindelia" href="./blog.html">blog</a> e veja um pouco sobre provas formais, tecnologia e outras coisas que gosto ou acho útil.
        </p>
      </div>
    </div>
  </section>
);

export default AboutSection;
