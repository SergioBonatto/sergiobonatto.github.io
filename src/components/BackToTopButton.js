import React, { useEffect } from 'react';
import '../CSS/BackToTopButton.css';

const BackToTopButton = () => {
  useEffect(() => {
    const handleScroll = () => {
      const backToTopButton = document.querySelector('.back-to-top');
      if (window.scrollY >= 360) {
        backToTopButton.classList.add('show');
      } else {
        backToTopButton.classList.remove('show');
      }
    };

    window.addEventListener('scroll', handleScroll);

    return () => {
      window.removeEventListener('scroll', handleScroll);
    };
  }, []);

  const scrollToTop = (e) => {
    e.preventDefault();
    window.scrollTo({ top: 0, behavior: 'smooth' });
  };

  return (
    <a href="#home" className="back-to-top" onClick={scrollToTop}>
      <i className="icon-arrow-up"></i>
    </a>
  );
};

export default BackToTopButton;
