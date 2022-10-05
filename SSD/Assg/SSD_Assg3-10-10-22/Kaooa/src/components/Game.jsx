import * as THREE from "three";
import { DragControls } from 'three/examples/jsm/controls/DragControls'
import { useEffect } from "react";

import SceneController from "../utils/SceneController";
import KaooaBoard from "./KaooaBoard";
import PlayerBoard from "./PlayerBoard";
import WinnerBoard from "./WinnerBoard";

export default function Game() {
    const canvasId = "kaooaCanvas";
    const bgcolor = "#000000";
    useEffect(()=>{
        /* ----------game scene and board init---------- */
        const game = new SceneController(canvasId, bgcolor);
        game.initScene();
        game.animate();

        const kboard = new KaooaBoard();
        var playerBoard = new PlayerBoard('CROW\'S TURN');

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
        game.scene.add(playerBoard.board);

        /* ----------mouse controls---------- */
    
        const controls = new DragControls(kboard.crowsAndVulture.children, game.camera, game.renderer.domElement);

        const mouse = new THREE.Vector2();
        const raycaster = new THREE.Raycaster();
        var currDraggedObj;
        var crowsTurn = true;
        var vulturePlayer = kboard.vulturePlayer;

        controls.addEventListener('dragstart', (event)=>{
            if((crowsTurn && event.object.player=='crow') || (!crowsTurn && event.object.player=='vulture')){
                currDraggedObj = event.object;
            }
            else {
                currDraggedObj = undefined;
            }
        });

        controls.addEventListener('dragend', (event)=>{
            event.object.position.x=event.object.x;
            event.object.position.y=event.object.y;
            event.object.position.z=event.object.z;
        });

        const onMouseUp = (event) => {
            mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
            mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
            raycaster.setFromCamera(mouse, game.camera);
            const intersects = raycaster.intersectObjects(kboard.hiddenTiles.children);
            if(intersects.length>0 && !intersects[0].object.isOccupied){
                var allNextMoves = currDraggedObj.nextMoves;
                var n = allNextMoves.length;
                var nextMoves = allNextMoves;
                var isMovePossible = true;

                if(currDraggedObj.player == 'vulture') nextMoves = nextMoves.slice(-1*n, -2);

                if(nextMoves.length!=0){
                    isMovePossible = false;
                    nextMoves.forEach((obj) => {
                        if(obj.uuid === intersects[0].object.uuid) {
                            isMovePossible = true;
                            return;
                        };
                    });
                }

                if(!isMovePossible && currDraggedObj.player=='vulture'){
                    var killMoves = allNextMoves.slice(-2);
                   
                    isMovePossible = false;
                    killMoves.forEach((obj) => {
                        if(obj.uuid === intersects[0].object.uuid) {
                            isMovePossible = true;
                            return;
                        };
                    });

                    if(isMovePossible){
                        
                    }
                    
                }
                

                if(isMovePossible){
                    currDraggedObj.position.x = intersects[0].object.x;
                    currDraggedObj.position.y = intersects[0].object.y;
                    currDraggedObj.x = intersects[0].object.x;
                    currDraggedObj.y = intersects[0].object.y;
                    intersects[0].object.isOccupied = true;
                    if(currDraggedObj.currPos!=undefined){
                        currDraggedObj.currPos.isOccupied = false;
                    }
                    currDraggedObj.currPos = intersects[0].object;
                    if(currDraggedObj.player=='crow') currDraggedObj.nextMoves = intersects[0].object.crowMoves;
                    else currDraggedObj.nextMoves = intersects[0].object.vultureMoves;

                    if(!crowsTurn) {
                        game.scene.remove(playerBoard.board);
                        playerBoard = new PlayerBoard('CROW\'S TURN!');
                    }
                    else {
                        game.scene.remove(playerBoard.board);
                        playerBoard = new PlayerBoard('VULTURE\'S TURN');
                    }
                    game.scene.add(playerBoard.board);

                    crowsTurn = !crowsTurn;
                }
            }

            if(!crowsTurn && vulturePlayer.nextMoves.length!=0){
                var didCrowsWin = true;
                vulturePlayer.nextMoves.forEach((obj) => {
                    if(obj.isOccupied == false){
                        didCrowsWin = false;
                    }
                })
                if(didCrowsWin){ 
                    game.scene.remove(game.scene.children[3]); 
                    game.scene.remove(game.scene.children[4]); 
                    game.scene.remove(playerBoard.board);
                    game.scene.add(new WinnerBoard('CROWS').board);
                }
            }
            
        }
        window.addEventListener("mouseup", onMouseUp, false);
    }, []);

    return (
        <div className="flex flex-col items-center justify-center">
            <canvas id={canvasId} />
        </div>
    );
}