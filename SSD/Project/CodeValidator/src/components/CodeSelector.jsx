import React from 'react'
import ListGroup from 'react-bootstrap/ListGroup';
import VanillaTilt from 'vanilla-tilt'

const CodeSelector = () => {
  VanillaTilt.init(document.querySelector(".codeselector"), {
      max: 25,
      speed: 400,
  });
  return (
    <div className='codeselector'>
    <ListGroup>
      <ListGroup.Item>Choose Code</ListGroup.Item>
      <div className='codes'>
      <ListGroup.Item action variant="dark">
        if
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        else
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        else if
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        break
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        while
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        for
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        return
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        if
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        else
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        else if
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        break
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        while
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        for
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        return
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        if
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        else
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        else if
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        break
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        while
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        for
      </ListGroup.Item>
      <ListGroup.Item action variant="dark">
        return
      </ListGroup.Item>
      </div>
      
    </ListGroup>
    </div>
  )
}

export default CodeSelector;