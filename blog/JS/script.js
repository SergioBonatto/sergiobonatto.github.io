// saudação por horário
// capturo o horario do dispositivo
let now = new Date()
let hora = now.getHours()
//defino os elementos que quero alterar
let oi = window.document.getElementById('text')
let satelite = window.document.getElementById('moon')
let estrela = window.document.getElementById('stars')

//se for entre 5 horas da manha e 12 horas
if (hora >= 5 && hora < 12) {
  // a mensagem é de bom dia
  oi.innerHTML = 'Bom dia!'
  //altero o  elemento satélite para o Sol
  satelite.src = './Imagens/parallax/sol.png'
  //defino o tamanho
  satelite.style.height = '100px'
  satelite.style.marginTop = '100px'
  // e a posição
  satelite.style.zIndex = '0'
  //mudo a cor de fundo para um azul similar ao céu de dia
  document.body.style.background = '#2fb3eb'
  // tiro as estrelas e coloco nuvens no lugar
  estrela.src = './Imagens/parallax/nuvens.png'
  //altero o tamanho das nuvens
  estrela.style.height = '90%'
  // e coloco na frente do sol
  estrela.style.zIndex = '1'
}
// se não, se for entre 12 horas e 18 da tarde
else if (hora >= 12 && hora < 18) {
  // mudo a mensagem para "Boa tarde"
  oi.innerHTML = 'Boa tarde!'
  // o resto permanece igual ao da manhã
  satelite.src = './Imagens/parallax/sol.png'
  satelite.style.height = '100px'
  satelite.style.marginTop = '100px'
  satelite.style.zIndex = '0'
  document.body.style.background = '#2fb3eb'
  estrela.src = './Imagens/parallax/nuvens.png'
  estrela.style.height = '90%'
  estrela.style.zIndex = '1'
}
// se não for manhã e nem tarde
else {
  // a mensagem vira "Boa noite"
  oi.innerHTML = 'Boa noite!'
  // a lua vem para a frente
  satelite.style.zIndex = '1'
  // e as estrelas vão para o fundo
  estrela.style.zIndex = '0'
}

//movimentação paralax
let paralax = () => {
  // defino os elementos que movimentarei
  let stars = document.getElementById('stars')
  let moon = document.getElementById('moon')
  let mountains_behind = document.getElementById('mountains_behind')
  let mountains_between = document.getElementById('mountains_between')
  let mountains_front = document.getElementById('mountains_front')
  let marco = document.getElementById('marco')
  let grama = document.getElementById('grama')
  let text = document.getElementById('text')

  //ao movimentar o scroll, cada elemento se moverá uma distancia
  window.addEventListener('scroll', () => {
    let value = window.scrollY
    stars.style.left = `${value * 0.25}px`
    stars.style.top = `${-value * 0.25}px`
    moon.style.top = `${value * 1.05}px`
    mountains_behind.style.top = `${value * 0.5}px`
    mountains_between.style.top = `${value * 0.75}px`
    mountains_front.style.top = `${value * 1}px`
    text.style.marginLeft = `${value * 3.5}px`
    text.style.marginTop = `${value * 0.75}px`
  })
}
paralax()

const nav = document.querySelector('#header nav')
const toggle = document.querySelectorAll('nav .toggle')

for (const element of toggle) {
  element.addEventListener('click', () => {
    nav.classList.toggle('show')
  })
}

// Quando clicar em um item, esconder o menu
const links = document.querySelectorAll('nav ul li a')

for (const link of links) {
  link.addEventListener('click', () => {
    nav.classList.remove('show')
  })
}

// Sombra no Header qdo for dado o scroll na pagina
const header = document.querySelector('#header')
const navHeight = header.offsetHeight

var changeHeaderWhenScroll = () => {
  if (window.scrollY >= navHeight) {
    // O scroll é maior que a altura do header
    header.classList.add('scroll')
  } else {
    // O scroll é menor que a altura do header
    header.classList.remove('scroll')
  }
}

//Back to top button, voltar par o topo
const backToTopButton = document.querySelector('.back-to-top')

var backToTop = () => {
  if (scrollY >= 360) {
    backToTopButton.classList.add('show')
  } else {
    backToTopButton.classList.remove('show')
  }
}

// Menu ativo conforme seção visível na página
const sections = document.querySelectorAll('main section[id]')

let activateMenuAtCurrentSection = () => {
  /* divide a alura da tela em 8, multiplica por 4 e soma com o deslocamento do eixo Y */
  const checkpoint = window.pageYOffset + (window.innerHeight / 8) * 4
  /* Para cada item na lista de sessões seção */
  for (let section of sections) {
    /* Ele cria uma variável */
    const sectionTop = section.offsetTop /* define o topo da seção */
    const sectionHeight = section.offsetHeight /* define a altura da seção */
    const sectionId =
      section.getAttribute('id') /* seleciona as IDs das seções */

    /* Cria o ponto de controle inicial da função */
    /* que deve ser o checkpoin desde que ele seja maior ou igual ao topo da seção */
    const checkpointStart = checkpoint >= sectionTop
    /* cria o ponto de controle final da função */
    /* que será quando o checkpoint for menor que o tamanho do topo mais a altura da seção, pegando a parte bem de baixo */
    const checkpointEnd = checkpoint <= sectionTop + sectionHeight
    /* Se o checkpoint estiver entre o checkpoint Start e o checkpointEnd */
    /* ou seja, com a seção visível */
    if (checkpointStart && checkpointEnd) {
      /* seleciona especificamente, na barra de navegação e dentro das listas, o link que tiver como referencia a id da seção visivel */
      document
        .querySelector(`nav ul li a[href*=${sectionId}]`)
        .classList.add('active')
    } else {
      document
        .querySelector(`nav ul li a[href*=${sectionId}]`)
        .classList.remove('active')
    }
  }
}

//When scroll
window.addEventListener('scroll', () => {
  changeHeaderWhenScroll()
  backToTop()
  activateMenuAtCurrentSection()
})


;(function() { "use strict";

var
	/**
	 * The parsed output string, in HTML format.
	 * @type {String}
	 */
	output = "",

	BLOCK = "block",
	INLINE = "inline",

	/**
	 * Used to attach MarkdownToHtml object to `window` in browser
	 * context, or as an AMD module where appropriate.
	 * @type {Object}
	 */
	exports,

	/**
	 * An array of parse rule descriptor objects. Each object has two keys;
	 * pattern (the RegExp to match), and replace (the replacement string or
	 * function to execute).
	 * @type {Array}
	 */
	parseMap = [
		{
			// <h1>
			// A line starting with 1-6 hashes.
			pattern: /(#{1,6})([^\n]+)/g,
			replace: "<h$L1>$2</h$L1>",
			type: BLOCK,
		},
		{
			// <p>
			// Any line surrounded by newlines that doesn't start with
			// an HTML tag, asterisk or numeric value with dot following.
			pattern: /\n(?!<\/?\w+>|\s?\*|\s?[0-9]+|>|\&gt;|-{5,})([^\n]+)/g,
			replace: "<p>$1</p>",
			type: BLOCK,
		},
		{
			// <blockquote>
			// A greater-than character preceding any characters.
			pattern: /\n(?:&gt;|\>)\W*(.*)/g,
			replace: "<blockquote><p>$1</p></blockquote>",
			type: BLOCK,
		},
		{
			// <ul>
			//
			pattern: /\n\s?\*\s*(.*)/g,
			replace: "<ul>\n\t<li>$1</li>\n</ul>",
			type: BLOCK,
		},
		{
			// <ol>
			//
			pattern: /\n\s?[0-9]+\.\s*(.*)/g,
			replace: "<ol>\n\t<li>$1</li>\n</ol>",
			type: BLOCK,
		},
		{
			// <strong>
			// Either two asterisks or two underscores, followed by any
			// characters, followed by the same two starting characters.
			pattern: /(\*\*|__)(.*?)\1/g,
			replace: "<strong>$2</strong>",
			type: INLINE,
		},
		{
			// <em>
			// Either one asterisk or one underscore, followed by any
			// characters, followed by the starting character.
			pattern: /(\*|_)(.*?)\1/g,
			replace: "<em>$2</em>",
			type: INLINE,
		},
		{
			// <a>
			// Not starting with an exclamation mark, square brackets
			// surrounding any characters, followed by parenthesis surrounding
			// any characters.
			pattern: /([^!])\[([^\[]+)\]\(([^\)]+)\)/g,
			replace: "$1<a href=\"$3\">$2</a>",
			type: INLINE,
		},
		{
			// <img>
			// Starting with an exclamation mark, then followed by square
			// brackets surrounding any characters, followed by parenthesis
			// surrounding any characters.
			pattern: /!\[([^\[]+)\]\(([^\)]+)\)/g,
			replace: "<img src=\"$2\" alt=\"$1\" />",
			type: INLINE,
		},
		{
			// <del>
			// Double tilde characters surrounding any characters.
			pattern: /\~\~(.*?)\~\~/g,
			replace: "<del>$1</del>",
			type: INLINE,
		},
		{
			// <code>
			//
			pattern: /`(.*?)`/g,
			replace: "<code>$1</code>",
			type: INLINE,
		},
		{
			// <hr>
			//
			pattern: /\n-{5,}\n/g,
			replace: "<hr />",
			type: BLOCK,
		},
	],
$$;

/**
 * Self-executing function to handle exporting the parse function for
 * external use.
 */
(function go() {
	// Export AMD module if possible.
	if(typeof module !== "undefined"
	&& typeof module.exports !== "undefined") {
		exports = module.exports;
	}
	// Otherwise check for browser context.
	else if(typeof window !== "undefined") {
		window.MarkdownToHtml = {};
		exports = window.MarkdownToHtml;
	}

	exports.parse = parse;
})();

/**
 * Parses a provided Markdown string into valid HTML.
 *
 * @param  {string} string Markdown input for transformation
 * @return {string}        Transformed HTML output
 */
function parse(string) {
	// Pad with newlines for compatibility.
	output = "\n" + string + "\n";

	parseMap.forEach(function(p) {
		// Replace all matches of provided RegExp pattern with either the
		// replacement string or callback function.
		output = output.replace(p.pattern, function() {
			// console.log(this, arguments);
			return replace.call(this, arguments, p.replace, p.type);
		});
	});

	// Perform any post-processing required.
	output = clean(output);
	// Trim for any spaces or newlines.
	output = output.trim();
	// Tidy up newlines to condense where more than 1 occurs back to back.
	output = output.replace(/[\n]{1,}/g, "\n");
	return output;
}

function replace(matchList, replacement, type) {
	var
		i,
	$$;

	for(i in matchList) {
		if(!matchList.hasOwnProperty(i)) {
			continue;
		}

		// Replace $n with the matching regexp group.
		replacement = replacement.split("$" + i).join(matchList[i]);
		// Replace $Ln with the matching regexp group's string length.
		replacement = replacement.split("$L" + i).join(matchList[i].length);
	}

	if(type === BLOCK) {
		replacement = replacement.trim() + "\n";
	}

	return replacement;
}

function clean(string) {
	var cleaningRuleArray = [
		{
			match: /<\/([uo]l)>\s*<\1>/g,
			replacement: "",
		},
		{
			match: /(<\/\w+>)<\/(blockquote)>\s*<\2>/g,
			replacement: "$1",
		},
	];

	cleaningRuleArray.forEach(function(rule) {
		string = string.replace(rule.match, rule.replacement);
	});

	return string;
}

})();
