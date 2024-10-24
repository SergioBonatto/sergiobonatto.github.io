import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import ReactMarkdown from 'react-markdown';

const BlogPost = () => {
  const { postId } = useParams(); // pega o id da URL
  const [content, setContent] = useState('');

  useEffect(() => {
    // Busca o conteúdo markdown com base no postId
    import(`../data/${postId}.md`)
      .then((res) => {
        fetch(res.default)
          .then((response) => response.text())
          .then((text) => setContent(text));
      })
      .catch((err) => console.error(err));
  }, [postId]);

  return (
    <article>
      <ReactMarkdown>{content}</ReactMarkdown>
    </article>
  );
};

export default BlogPost;
