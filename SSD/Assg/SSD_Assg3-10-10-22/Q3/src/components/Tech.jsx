import NewsCard from "./NewsCard";
import axios from "axios";
import React, { useState, useEffect } from "react";
import SearchBar from "./SearchBar";
import Advertisement from "./Advertisement";

const Tech = () => {
  const [news, setNews] = useState([]);
  const API_KEY = "defb7e7d42cd4e978aba3ac5876862e3";
  var id = 1;
  var data;

  const fetchNews = async () => {
    axios
      // .get(`https://newsapi.org/v2/top-headlines?country=us&apiKey=${API_KEY}`)
      .get("src/assets/techcrunch.json")
      .then((response) => {
        if (news.length == 0) setNews(response.data);
      })
      .catch((error) => {
        if (error.response) console.log(error.response.status);
        else console.log(`Error: ${error.message}`);
      });
  };

  useEffect(() => {
    fetchNews();
  }, [news]);

  return (
    <div>
      <Advertisement ad="ad2" />
      <div className="flex">
        <div className="flex flex-row flex-wrap">
          {news.totalResults > 0
            ? news.articles.map((article) => {
                if (article.urlToImage != null) {
                  return (
                    <NewsCard
                      key={id++ * 10}
                      id={id}
                      urlToImage={article.urlToImage}
                      publishedAt={article.publishedAt}
                      publishedBy={article.source.name}
                      description={article.description}
                      content={article.content}
                      url={article.url}
                    />
                  );
                }
              })
            : console.log(news.articles)}
        </div>
      </div>
    </div>
  );
};

export default Tech;
