import React, { useEffect } from 'react'
import CodeMirror from '@uiw/react-codemirror';
// import { javascript } from '@codemirror/lang-javascript';

const Editor = () => {
    const onChange = React.useCallback((value, viewUpdate) => {
        console.log('value:', value);
      }, []);
      return (
        <CodeMirror
          className='editor'
          value="write your code here"
          height="750px"
          width="800px"
          theme="dark"
        //   extensions={[javascript({ jsx: true })]}
          onChange={onChange}
        />
      );
}

export default Editor;
