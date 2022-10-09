import React, { Component, useState, useEffect } from "react";
import Carousel from "react-elastic-carousel";
import axios from "axios";
import Item from "./Item";

const breakpoints = [{ width: 1, itemsToShow: 1 }];

const Advertisement = (props) => {
  const [ad, setAd] = useState([]);
  var id = 1;

  const fetchAd = async () => {
    axios
      .get(`src/assets/${props.ad}.json`)
      .then((response) => {
        if (ad.length == 0) setAd(response.data);
      })
      .catch((error) => {
        if (error.response) console.log(error.response.status);
        else console.log(`Error: ${error.message}`);
      });

    console.log(ad);
  };

  useEffect(() => {
    fetchAd();
  }, [ad]);

  return (
    <>
      <div className="p-5">
        <Carousel breakPoints={breakpoints}>
          {ad.totalResults > 0
            ? ad.ads.map((a) => {
                return (
                  <Item key={10 * id++} id={id}>
                    <img src={a.urlToImage} />
                  </Item>
                );
              })
            : console.log("no ads")}
          <div></div>
        </Carousel>
      </div>
    </>
  );
};

export default Advertisement;
