import React, { Component } from 'react';
import { Routes ,Route } from 'react-router-dom';
import Login from '../components/Login';
import Student from '../components/Student';
import { useNavigate } from "react-router-dom";

import useAuth from "../utils/useAuth";

function AllRoutes() {
  return (
    <Routes>
        <Route path="/login" element={<RequireAuth><Login/></RequireAuth>}/>
        <Route path="/student" element={<Student/>}/>
    </Routes>
  )
}

function RequireAuth({ children }) {
  const { authed } = useAuth();
  return authed === true ? children : <Navigate to="/login" replace />;
}

export default AllRoutes
