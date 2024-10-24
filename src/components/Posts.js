import React from 'react';
import { Link } from 'react-router-dom';
import posts from '../data/posts.json';  // Importe o JSON com os metadados dos posts

const Posts = () => {
  return (
    <section>
      {posts.map((post) => (
        <article key={post.id}>
          <img src={post.image} alt={post.title} />
          <h2>{post.title}</h2>
          <p>{post.summary}</p>
          <Link to={`/blog/${post.id}`}>Read More</Link> {/* Link para o post individual */}
        </article>
      ))}
    </section>
  );
};

export default Posts;
