// Função para mostrar o botão de voltar ao topo
function backToTop() {
  const backToTopButton = document.querySelector('.back-to-top');

  if (scrollY >= 360) {
    backToTopButton.classList.add('show');
  } else {
    backToTopButton.classList.remove('show');
  }
}



// Inicialização das funções

window.addEventListener('scroll', () => {
  backToTop();
});
