import React from 'react';
import HeaderComponent from './components/HeaderComponent';
import AboutSection from './components/AboutSection';
import ContactSection from './components/ContactSection';
import FooterComponent from './components/FooterComponent';
import ParallaxSection from './components/ParallaxSection';
import './CSS/App.css';

function App() {
  return (
    <div className="App">
      <HeaderComponent />
      <main>
        <ParallaxSection />
        <AboutSection />
        <ContactSection />
      </main>
      <FooterComponent />
      <a href="#home" className="back-to-top">
        <i className="icon-arrow-up"></i>
      </a>
    </div>
  );
}

export default App;
