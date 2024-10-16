// Caminho para o arquivo markdown
const markdownFile = './my_terminal.md';

// Elemento onde o conteúdo markdown será injetado
const contentElement = document.getElementById('blog');

// Função para carregar e converter markdown
fetch(markdownFile)
  .then(response => response.text())
  .then(markdown => {
    // Converter o markdown para HTML
    const htmlContent = marked(markdown);
    // Inserir o conteúdo HTML no elemento
    contentElement.innerHTML = htmlContent;
  })
  .catch(error => console.error('Erro ao carregar o arquivo markdown:', error));
