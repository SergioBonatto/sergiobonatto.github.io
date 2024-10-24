import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import ReactMarkdown from 'react-markdown';
import '../CSS/BlogPost.css';

const BlogPost = () => {
  const { postId } = useParams();
  const [content, setContent] = useState('');

  useEffect(() => {
    import(`../data/${postId}.md`)
      .then((res) => {
        fetch(res.default)
          .then((response) => response.text())
          .then((text) => setContent(text));
      })
      .catch((err) => console.error(err));
  }, [postId]);

  return (
    <article className="blog-post">
      <ReactMarkdown>{content}</ReactMarkdown>
    </article>
  );
};

export default BlogPost;
