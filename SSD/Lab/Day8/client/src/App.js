import { useState } from "react";
import  {BrowserRouter as Router} from "react-router-dom";
import AllRoutes from "./routes/AllRoutes";
import AuthApi from "./utils/useAuth";

function App() {

  const [auth, setAuth] = useState(false);

  return (
    <div className="App">
      <AuthApi.Provider value = {{auth, setAuth}}>
        <Router>
          <AllRoutes />
        </Router>
      </AuthApi.Provider>
    </div>
  );
}

export default App;

