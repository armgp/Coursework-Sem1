import * as THREE from "three";
import { DragControls } from 'three/examples/jsm/controls/DragControls'
import { useEffect } from "react";

import SceneController from "../utils/SceneController";
import KaooaBoard from "./KaooaBoard";

export default function Game() {
    const canvasId = "kaooaCanvas";
    const bgcolor = "#000000";
    useEffect(()=>{
        /* ----------game scene and board init---------- */
        const game = new SceneController(canvasId, bgcolor);
        game.initScene();
        game.animate();

        const kboard = new KaooaBoard();
        const directionalLight3 = new THREE.DirectionalLight(0x8F35C2, 1);
        const directionalLight1 = new THREE.DirectionalLight(0x29C29E, 1);
        const directionalLight2 = new THREE.DirectionalLight(0xC91111, 1);
        directionalLight2.position.set(1, 0, 0);
        directionalLight3.position.set(0, 0, 1);
        // const light = new THREE.HemisphereLight(0xffffbb, 0x080820, 0.4);
        // game.scene.add(light);
        game.scene.add(directionalLight1);
        game.scene.add(directionalLight2);
        game.scene.add(directionalLight3);

        game.scene.add(kboard.board);

        const grow = (object) => {
            if(object.scale.x<1) object.scale.x+=0.04;
            if(object.scale.y<1) object.scale.y+=0.04;
            if(object.scale.z<1) object.scale.z+=0.04;
        }

        var growthIndex = 0.0002;
        const animateEntry = () => {
            if(kboard.board.scale.x < 1) {
                kboard.board.scale.x+=growthIndex;
                kboard.board.rotation.z += 0.03;
            }
            if(kboard.board.scale.y < 1) kboard.board.scale.y+=growthIndex;
            if(kboard.board.scale.z < 1) kboard.board.scale.z+=growthIndex;
            growthIndex*=2;
            requestAnimationFrame(animateEntry);
        };
        
        const animateBoard = () => {
            if(kboard.board.scale.x < 1) {
                // kboard.board.rotation.x += 0.01;
                // kboard.board.rotation.y += 0.01;
                kboard.board.rotation.z += 0.003;
                requestAnimationFrame(animateBoard);
            }
        };
        
        animateBoard();
        animateEntry();




        /* ----------mouse controls---------- */
        
        
        

        const controls = new DragControls(kboard.crowsAndVulture.children, game.camera, game.renderer.domElement);
        
        // controls.addEventListener('dragstart', (event)=>{
        //     event.object.material.emissive = 0xaaaaaa;
        // });

        const mouse = new THREE.Vector2();
        const raycaster = new THREE.Raycaster();
        var currDraggedObj;
        controls.addEventListener('dragend', (event)=>{
            currDraggedObj = event.object;
            event.object.position.x=event.object.x;
            event.object.position.y=event.object.y;
            event.object.position.z=event.object.z;
        });

        var nextMoves;
        const onMouseDown = (event) => {
            mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
            mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
            raycaster.setFromCamera(mouse, game.camera);
            const intersects = raycaster.intersectObjects(kboard.hiddenTiles.children);
            console.log("out");
            console.log(intersects);
            if(intersects.length > 1 && intersects[0].object.isOccupied){
                intersects[0].object.isOccupied = false;
                if(intersects[0].object.player == 'crow'){
                    nextMoves = intersects[0].object.crowMoves;
                    console.log(nextMoves);
                }else{
                    nextMoves = intersects[0].object.vultureMoves;
                }
            }
        }
        window.addEventListener("mousedown", onMouseDown, false);

        const onMouseUp = (event) => {
            mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
            mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
            raycaster.setFromCamera(mouse, game.camera);
            const intersects = raycaster.intersectObjects(kboard.hiddenTiles.children);
            
            console.log(intersects);
            if(intersects.length > 0 && !intersects[0].object.isOccupied && currDraggedObj!=undefined){
                var isMovePossible = true;
                if(nextMoves!=undefined && nextMoves!=[]){
                    console.log("inside");
                    console.log(nextMoves);
                    isMovePossible = false;
                    nextMoves.forEach((obj) => {
                        if(obj.uuid === intersects[0].object.uuid) {
                            isMovePossible = true;
                            return;
                        };
                    });
                }

                console.log(isMovePossible);

                if(isMovePossible){
                    currDraggedObj.position.x = intersects[0].object.x;
                    currDraggedObj.position.y = intersects[0].object.y;
                    currDraggedObj.x = intersects[0].object.x;
                    currDraggedObj.y = intersects[0].object.y;
                    intersects[0].object.isOccupied = true;
                    intersects[0].object.player = currDraggedObj.player;
                    nextMoves = undefined;
                }
            }
            currDraggedObj=undefined;
        }
        window.addEventListener("mouseup", onMouseUp, false);

        
        // window.addEventListener("mousemove", onMouseMove, false);

    }, []);

    return (
        <div className="flex flex-col items-center justify-center">
            <canvas id={canvasId} />
        </div>
    );
}