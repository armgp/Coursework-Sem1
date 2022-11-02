import Button from 'react-bootstrap/Button';
import React, { Component } from 'react'

export default class UploadButton extends Component {

    onChange(e){
        let files = e.target.files;
        let reader = new FileReader();
        // reader.readAsDataURL(files[0]);
        reader.readAsText(files[0]);
        reader.onload = (e) => {
            console.warn("json data ", e.target.result);
            const url = "";
            // const jsonData = {file: e.target.result};
            const jsonData = e.target.result;
            console.error(jsonData);
            // return post(url, jsonData).then(response=>console.warn("result", response))
        }
    }

    render() {
        return (
            <div className='uploaddiv'>
                Upload Syntax File
                <Button className='uploadbutton' variant="secondary">
                    <input type="file" name="file" onChange={(e)=>this.onChange(e)} />
                </Button>
            </div>
        )
    }
}
