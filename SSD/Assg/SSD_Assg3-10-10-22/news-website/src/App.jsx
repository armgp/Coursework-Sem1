import AllNews from "./components/AllNews";
import Tech from "./components/Tech";
import Business from "./components/Business";
import Wallstreet from "./components/Wallstreet";
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import "./App.css";
import SearchBar from "./components/SearchBar";
import Advertisement from "./components/Advertisement";

function App() {
  return (
    <div>
      <Router>
        <SearchBar />
        <Routes>
          <Route path="/" element={<AllNews />} />
          <Route path="/business" element={<Business />} />
          <Route path="/tech" element={<Tech />} />
          <Route path="/wallstreet" element={<Wallstreet />} />
        </Routes>
      </Router>
    </div>
  );
}

export default App;
