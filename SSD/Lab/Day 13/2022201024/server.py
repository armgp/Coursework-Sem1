from flask import Flask, request, render_template, url_for, redirect
from flask_sqlalchemy import SQLAlchemy
from flask_login import (LoginManager, login_manager, login_user, logout_user, login_required, current_user, UserMixin)
from flask_wtf import FlaskForm 
from wtforms import StringField, PasswordField, SubmitField
from wtforms.validators import InputRequired, Length, ValidationError
from flask_bcrypt import Bcrypt

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///database.db'
app.config['SECRET_KEY'] = 'secretkey'
db = SQLAlchemy(app)

login_manager = LoginManager()
login_manager.init_app(app)

class User(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key = True)
    name = db.Column(db.String(20), nullable = False)
    email = db.Column(db.String(20), nullable = False, unique = True)
    password = db.Column(db.String(20), nullable = False)

@app.before_first_request
def create_tables():
    db.create_all()

@app.route("/user/signin", methods=['POST'])
def signin():
    if(request.method == 'POST'):
        req = request.get_json()
        email = req['email']
        password = req['password']
        check_user = User.query.filter_by(email=email).first()
        if(check_user is not None):
            if(check_user.password == password):
                login_user(check_user)
                return "Logged in successfully"
            else:
                return "Wrong Password"
        else:
            return "User Doesn't Exist"
        
    

@app.route("/user/signup", methods=['POST'])
def signup():
    if(request.method == 'POST'):
        req = request.get_json()
        name = req['name']
        email = req['email']
        password = req['password']

        existing_email = User.query.filter_by(email=email).first()
        existing_name = User.query.filter_by(name=name).first()

        if existing_email:
            raise ValidationError("Email already registered with us")

        
        elif existing_name:
            raise ValidationError("username already exists, choose a different username")

        else:
            new_user = User(name=name, email=email, password=password)
            db.session.add(new_user)
            db.session.commit()
            return "Successfully Registered"

@login_manager.user_loader
def load_user(user):
    return User.query.get(int(user))

@app.route("/user/signout", methods=['GET'])
def signout():
    user = current_user.get_id()
    logout_user()
    return "User logged out"


if __name__ == '__main__':
    app.run(debug = True)