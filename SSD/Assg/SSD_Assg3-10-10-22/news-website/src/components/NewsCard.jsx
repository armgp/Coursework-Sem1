import React from "react";

const NewsCard = (props) => {
  return (
    <div
      id={props.id}
      className="px-2 md:px-6 my-3 max-w-xl text-slate-700 dark:text-white flex flex-col items-center"
    >
      <div className="max-w-l min-h-[700px] text-left flex flex-col items-center justify-center md:p-4 md:border border-slate-300 dark:border-slate-600 rounded-xl">
        <div className="w-full rounded-xl flex-col xl:flex-row bg-white dark:bg-slate-900 shadow-md">
          <div className="rounded-t-xl w-full shadow-sm bg-cover">
            <img src={props.urlToImage} />
          </div>

          <div className="w-full p-3 flex flex-col justify-between h-auto overflow-auto lg:h-auto">
            <h1 className="text-left text-sm md:text-lg font-bold leading-normal">
              {props.content}
            </h1>
            <p className="text-sm">{props.description}.</p>

            <div className="flex mt-4">
              <button
                onClick={(e) => {
                  e.preventDefault();
                  window.open(props.url, "_blank");
                }}
                className="transition-all duration-100 text-center p-2 rounded-md text-white w-1/2 bg-gradient-to-r from-blue-700 to-blue-500 hover:shadow-md hover:from-blue-800 hover:to-blue-600"
              >
                View Full-Size
              </button>
              <div className="flex flex-col ml-4 w-1/2">
                <h2 className="text-center text-xs mt-1 mb-2 text-blue-600 dark:text-blue-400 font-bold uppercase">
                  Published by {props.publishedBy}
                </h2>

                <span className="self-center text-xs text-blue-700 dark:text-blue-300 -mt-2">
                  {props.publishedAt}
                </span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default NewsCard;
