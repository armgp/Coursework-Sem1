require("dotenv").config();

const express = require('express');
const app = express();

app.get("/api", (req, res) => {
    res.json({"users": ["Yes", "No"]});
});

app.listen(5000, () => {
    console.log("Server started at port 5000");
    console.log(process.env.NAME);
});

// require("dotenv").config();

// var bodyParser = require("body-parser");
// var cookieParser = require("cookie-parser");
// var session = require("express-session");
// var morgan = require("morgan");

// const express = require('express');
// const app = express();

// app.use(bodyParser.urlencoded({extended: true}));
// app.use(cookieParser());
// app.use(
//     session({
//       key: "user_id",
//       secret: "somerandonstuffs",
//       resave: false,
//       saveUninitialized: false,
//       cookie: {
//         //6 days
//         expires: 600000,
//       },
//     })
// );

// app.use((req, res, next) => {
//     if(req.session.user && req.cookies.user_id){
//         res.redirect('/student')
//     }
//     next();
// })

// var sessionChecker = (req, res, next) => {
//     if(req.session.user && req.cookies.user_id){
//         res.redirect('/student')
//     }else next();
// }

// app.get("/", sessionChecker, (req, res) => {
//     res.redirect('/login');
// })

// app.get("/api", (req, res) => {
//     res.json({"users": ["Yes", "No"]});
// });

// app.listen(5000, () => {
//     console.log("Server started at port 5000");
//     console.log(process.env.NAME);
// });