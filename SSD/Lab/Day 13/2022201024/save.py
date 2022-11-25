from flask import Flask, render_template, url_for, redirect
from flask_sqlalchemy import SQLAlchemy
from flask_login import UserMixin
from flask_wtf import FlaskForm 
from wtforms import StringField, PasswordField, SubmitField
from wtforms.validators import InputRequired, Length, ValidationError
from flask_bcrypt import Bcrypt

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///database.db'
app.config['SECRET_KEY'] = 'secretkey'
db = SQLAlchemy(app)

class User(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key = True)
    name = db.Column(db.String(20), nullable = False)
    email = db.Column(db.String(20), nullable = False, unique = True)
    password = db.Column(db.String(20), nullable = False)

# class SignupForm(FlaskForm):
#     name = StringField(validators=[InputRequired(), Length(min=4, max=20)], render_kw={"placeholder": "name"})
#     email = StringField(validators=[InputRequired(), Length(min=4, max=20)], render_kw={"placeholder": "email"})
#     password = PasswordField(validators=[InputRequired(), Length(min=4, max=20)], render_kw={"placeholder": "password"})
#     submit = SubmitField("Register")

#     def validateEmail(self, email):
#         existing_email = User.query.filter_by(email=email.data).first()
#         if existing_email:
#             raise ValidationError("Email already registered with us")
    
#     def validateUserName(self, name):
#         existing_name = User.query.filter_by(name=name.data).first()
#         if existing_name:
#             raise ValidationError("username already exists, choose a different username")

# class LoginForm(FlaskForm):
#     email = StringField(validators=[InputRequired(), Length(min=4, max=20)], render_kw={"placeholder": "email"})
#     password = PasswordField(validators=[InputRequired(), Length(min=4, max=20)], render_kw={"placeholder": "password"})
#     submit = SubmitField("Login")

@app.route("/")
def welcome():
    return "hello"

@app.route("/login", methods=['GET', 'POST'])
def login():
    # form = LoginForm()
    # return render_template('login.html', form = form)

@app.route("/user/signup", methods=['POST'])
def signup():
    # form = SignupForm()
    # if form.validate_on_submit():
    #     hashed_passw = Bcrypt.generate_password_hash(form.password.data)
    #     new_user = User(name=form.name.data, email=form.email.data, password=hashed_passw)
    #     db.session.add(new_user)
    #     db.session.commit()
    #     return redirect(url_for('login'))
    # return render_template('signup.html', form = form)

if __name__ == '__main__':
    app.run(debug = True)