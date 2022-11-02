import React, { useEffect, useState } from 'react'
import ListGroup from 'react-bootstrap/ListGroup';
import VanillaTilt from 'vanilla-tilt'
import jsonData from '../assets/response.json'

const CodeSelector = () => {
  VanillaTilt.init(document.querySelector(".codeselector"), {
      max: 25,
      speed: 400,
  });

  const [data, setData] = useState([]);

  useEffect(()=>{
    const getData = async () => {
      const loadData = JSON.parse(JSON.stringify(jsonData));
      setData(loadData.statements);
    };

    getData();
    
    console.log(data);
  },[])

  return (
    <div className='codeselector'>
    <ListGroup>
      <ListGroup.Item>Choose Code</ListGroup.Item>
      <div className='codes'>
      {data.length>0 ? data.map((statement, num) => {
          return (<ListGroup.Item key={num} action variant="dark">
            {statement.title}
            </ListGroup.Item>)
        }) :
        <ListGroup.Item action variant="dark">
          No Data Available
         </ListGroup.Item>
        }
      </div>
      
    </ListGroup>
    </div>
  )
}

export default CodeSelector;