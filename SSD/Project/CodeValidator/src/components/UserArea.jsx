import React from 'react'
import CodeSelector from './CodeSelector'
import Editor from './Editor'
import UploadButton from './UploadButton'
import ValidateButton from './ValidateButton'

const UserArea = () => {
  return (
    <div>
    <UploadButton/>
    <div className='userarea'>
        <CodeSelector/>
        <Editor/>
        <ValidateButton/>
    </div>
    </div>
  )
}

export default UserArea