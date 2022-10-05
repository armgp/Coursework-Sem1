import { useState } from 'react'
import reactLogo from './assets/react.svg'
import './App.css'
import Game from './components/Game'

function App() {
  return (
    <Game />
  )
}

export default App

// import { BrowserRouter as Router,Routes, Route, Link } from 'react-router-dom';
// import Game from './components/Game';
  
// function App() {
//     return (
//        <Router>
//            <div className="App">
//                 <Link to="/game">Start</Link>
              
//            <Routes>
//                  <Route exact path='/game' element={< Game />}></Route>
//           </Routes>
//           </div>
//        </Router>
//    );
// }
  
// export default App;