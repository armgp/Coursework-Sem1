import React, { Component } from 'react'
import Feedback from './Feedback'

export default class Student extends Component {
  render() {
    return (
      <div>
        <button>Add New Query</button>
        <Feedback />
      </div>
    )
  }
}
