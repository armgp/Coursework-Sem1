import React, { Component } from 'react'

export default class Login extends Component {

    handleInputChange(event) {
      event.preventDefault();
      const target = event.target;
      this.setState({
        [target.name]: target.value,
      });
    }
  
    handleSubmit(event) {
      event.preventDefault();
      //code here
    }

    handleSignUp(event) {
      event.preventDefault();
      //code here
    }

  render() {
    return (
      <div>
          <form onSubmit={this.handleSubmit}>
            <label>
              Roll Number
              <input
                name="emailOrUsername"
                type="text"
                placeholder='Roll Number'
                // value={this.state.emailOrUsername}
                // onChange={this.handleInputChange}
              />
            </label>
            <label>
              Password
              <input
                name="password"
                type="password"
                placeholder='Password'
                // value={this.state.password}
                // onChange={this.handleInputChange}
              />
            </label>
            <label>
              Select Role
              <select id="role" name="role">
                <option value="student">Student</option>
                <option value="ta">TA</option>
              </select>
            </label>
            <button type="submit">Log in</button>
            <button onClick={this.handleSignUp}>Sign up</button>
          </form>
        </div>
    )
  }
}
