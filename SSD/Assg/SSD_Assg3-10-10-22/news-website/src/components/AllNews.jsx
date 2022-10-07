import NewsCard from "./NewsCard";
import axios from "axios";
import React, { useState, useEffect } from "react";

const AllNews = () => {
  const [keyword, setKeyword] = useState("");
  const [news, setNews] = useState([]);
  const [url, setUrl] = useState(
    "https://newsapi.org/v2/top-headlines?country=us&apiKey="
  );
  const API_KEY = "defb7e7d42cd4e978aba3ac5876862e3";
  const mainnewsUrl = "https://newsapi.org/v2/top-headlines?country=us&apiKey=";
  const searchUrl = `https://newsapi.org/v2/everything?q=`;
  var id = 1;

  const fetchNews = async () => {
    axios
      .get(`${url}${API_KEY}`)
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

  const handleSubmit = (event) => {
    event.preventDefault();
    // console.log(keyword);
    setUrl(`${searchUrl}${keyword}&from=2022-10-07&sortBy=popularity&apiKey=`);
    fetchNews();
    // console.log(news);
  };

  return (
    <div>
      <div className="max-w-2xl mx-auto">
        <form onSubmit={handleSubmit} className="flex items-center">
          <label htmlFor="simple-search" className="sr-only">
            Search
          </label>
          <div className="relative w-full">
            <div className="flex absolute inset-y-0 left-0 items-center pl-3 pointer-events-none">
              <svg
                className="w-5 h-5 text-gray-500 dark:text-gray-400"
                fill="currentColor"
                viewBox="0 0 20 20"
                xmlns="http://www.w3.org/2000/svg"
              >
                <path
                  fillRule="evenodd"
                  d="M8 4a4 4 0 100 8 4 4 0 000-8zM2 8a6 6 0 1110.89 3.476l4.817 4.817a1 1 0 01-1.414 1.414l-4.816-4.816A6 6 0 012 8z"
                  clipRule="evenodd"
                ></path>
              </svg>
            </div>
            <input
              // id="first_name"
              // name="first_name"
              // type="text"
              // onChange={event => setFirstName(event.target.value)}
              // value={firstName}
              type="text"
              id="simple-search"
              name="simple-search"
              onChange={(event) => setKeyword(event.target.value)}
              className="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full pl-10 p-2.5  dark:bg-gray-700 dark:border-gray-600 dark:placeholder-gray-400 dark:text-white dark:focus:ring-blue-500 dark:focus:border-blue-500"
              placeholder="Search"
              required
            />
          </div>
          <button
            type="submit"
            className="p-2.5 ml-2 text-sm font-medium text-white bg-blue-700 rounded-lg border border-blue-700 hover:bg-blue-800 focus:ring-4 focus:outline-none focus:ring-blue-300 dark:bg-blue-600 dark:hover:bg-blue-700 dark:focus:ring-blue-800"
          >
            <svg
              className="w-5 h-5"
              fill="none"
              stroke="currentColor"
              viewBox="0 0 24 24"
              xmlns="http://www.w3.org/2000/svg"
            >
              <path
                strokeLinecap="round"
                strokeLinejoin="round"
                strokeWidth="2"
                d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0z"
              ></path>
            </svg>
          </button>
        </form>
        <script src="https://unpkg.com/flowbite@1.4.0/dist/flowbite.js"></script>
      </div>
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

export default AllNews;
