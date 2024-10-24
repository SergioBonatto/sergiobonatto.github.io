import React from 'react';
import { BrowserRouter as Router, Route, Routes } from 'react-router-dom';
import BackToTopButton from './components/BackToTopButton';
import HeaderComponent from './components/HeaderComponent';
import AboutSection from './components/AboutSection';
import ContactSection from './components/ContactSection';
import FooterComponent from './components/FooterComponent';
import ParallaxSection from './components/ParallaxSection';
import Posts from './components/Posts';
import BlogPost from './components/BlogPost';
import NotFoundPage from './components/NotFoundPage';
import './CSS/App.css';



function App() {
  return (
    <Router>
      <Routes>
        {/* Rotas principais */}
        <Route
          path="/"
          element={
            <>
              <HeaderComponent />
              <ParallaxSection />
              <AboutSection />
              <ContactSection />
              <FooterComponent />
              <BackToTopButton />
            </>
          }
        />
        <Route
          path="/posts"
          element={
            <>
              <HeaderComponent />
              <Posts />
              <FooterComponent />
            </>
          }
        />
        <Route
          path="/blog/:postId"
          element={
            <>
              <HeaderComponent />
              <BlogPost />
              <FooterComponent />
            </>
          }
        />
        {/* Página 404 */}
        <Route path="*" element={<NotFoundPage />} />
      </Routes>
    </Router>
  );
}

export default App;
