import * as THREE from "three";
import { FontLoader } from 'three/examples/jsm/loaders/FontLoader.js';
import { TextGeometry } from 'three/examples/jsm/geometries/TextGeometry.js';

export default class DownloadEvents {
    constructor(){
        this.isvis = false;
        this.board = new THREE.Group();
        const fontLoader = new FontLoader();
        fontLoader.load('src/assets/Karma_Future_Regular.json', (font)=>{
            const geometry = new TextGeometry("Download Events", {
                font: font,
                size: 3,
                height: 2
            });

            const textMesh = new THREE.Mesh(geometry, [
                new THREE.MeshToonMaterial({color: 0x007007}), //front
                new THREE.MeshToonMaterial({color: 0x5c2301}) //side
            ]);

            textMesh.castShadow = true;
            textMesh.scale.x = 3.5;
            textMesh.scale.y = 3.5;
            textMesh.scale.z = 3.5;
            textMesh.position.x = -300;
            textMesh.position.y = -100;

            this.board.add(textMesh);
        });
    }

}
