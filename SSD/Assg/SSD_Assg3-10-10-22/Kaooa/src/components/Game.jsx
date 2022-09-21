import * as THREE from "three";
import { useEffect } from "react";

import SceneController from "../utils/SceneController";

export default function Game() {
    const canvasId = "kaooaCanvas";
    const bgcolor = "#a3a4f5";
    useEffect(()=>{
        const game = new SceneController(canvasId, bgcolor);
        game.initScene();
        game.animate();

        //call class for board here
        const geometry = new THREE.BoxGeometry( 1, 1, 1 );
        const material = new THREE.MeshPhongMaterial( { color: 0x570ef7 } );
        const light = new THREE.HemisphereLight( 0xffffbb, 0x080820, 1 );
        game.scene.add( light );
        const cube = new THREE.Mesh( geometry, material );
        //add board here
        game.scene.add(cube);
        
        const animate = () => {
            cube.rotation.x += 0.01;
            cube.rotation.y += 0.01;
            requestAnimationFrame(animate);
        };

        // animate();

    }, []);

    return (
        <div className="flex flex-col items-center justify-center">
            <canvas id={canvasId} />
        </div>
    );
}