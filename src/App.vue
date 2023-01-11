<template>
  <canvas class="w-screen h-screen overflow-hidden"></canvas>
</template>

<script lang="ts">
import * as THREE from 'three';
import { computed, onMounted, watch } from 'vue';
import { useWindowSize } from '@vueuse/core';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import Planet from './components/Planet';
import { Plane } from 'three';

export default {
  setup() {
    const scene = new THREE.Scene();
    scene.background = new THREE.TextureLoader().load('space.jpg');
    const { width, height } = useWindowSize();
    let renderer: THREE.WebGLRenderer;
    let camera: THREE.PerspectiveCamera;
    let controls: OrbitControls;
    const aspectRatio = computed(() => width.value / height.value);

    function updateRenderer() {
      renderer.setSize(width.value, height.value);
      renderer.setPixelRatio(window.devicePixelRatio);
    }

    function updateCamera() {
      camera.aspect = aspectRatio.value;
      camera.updateProjectionMatrix();
      camera.frustumCulled = false;
    }

    watch(aspectRatio, () => {
      updateCamera();
      updateRenderer();
    });
    camera = new THREE.PerspectiveCamera(50, aspectRatio.value, 0.1, 100000);
    camera.position.z = 700;
    const sunTexture = new THREE.TextureLoader().load('sun.jpg');
    const sun = new THREE.Mesh(new THREE.SphereGeometry(60, 90, 90), new THREE.MeshBasicMaterial({ map: sunTexture }));
    const light1 = new THREE.PointLight(0xffffff, 1.2, 10000);
    light1.position.set(0, 0, 0);
    light1.rotateY(Math.PI / 2);
    light1.castShadow = true; // default false

    scene.add(light1);
    // const helper = new THREE.CameraHelper(light1.shadow.camera);
    // light1.add(helper);
    const solarSystem = new THREE.Group();

    const mercury = new Planet(2, 95, 'mercury.jpg');
    const mercuryMesh = mercury.getMesh();
    let mercurySystem = new THREE.Group();
    mercurySystem.rotation.y = Math.random() * Math.PI * 2;
    mercurySystem.add(mercuryMesh);

    const venus = new Planet(5.8, 127, 'venus.jpg');
    const venusMesh = venus.getMesh();
    let venusSystem = new THREE.Group();
    venusSystem.rotation.y = Math.random() * Math.PI * 2;
    venusSystem.add(venusMesh);

    const earth = new Planet(6, 153, 'earth.jpg');
    const earthMesh = earth.getMesh();
    let earthSystem = new THREE.Group();
    earthSystem.rotation.y = Math.random() * Math.PI * 2;
    earthSystem.add(earthMesh);

    const mars = new Planet(3, 202, 'mars.jpg');
    const marsMesh = mars.getMesh();
    let marsSystem = new THREE.Group();
    marsSystem.rotation.y = Math.random() * Math.PI * 2;
    marsSystem.add(marsMesh);

    const jupiter = new Planet(20, 544, 'jupiter.jpg');
    const jupiterMesh = jupiter.getMesh();
    let jupiterSystem = new THREE.Group();
    jupiterSystem.rotation.y = Math.random() * Math.PI * 2;
    jupiterSystem.add(jupiterMesh);

    const saturn = new Planet(17, 949, 'saturn.jpg');
    const saturnMesh = saturn.getMesh();
    const saturnRingTexture = new THREE.TextureLoader().load('rings.jpg', function (texture) {
      texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
      texture.offset.set(0, 0);
      texture.repeat.set(1, 1);
    });
    const saturnRingGeo = new THREE.RingGeometry(20, 30, 80);
    let uvs = saturnRingGeo.attributes.uv.array;
    // loop and initialization taken from RingBufferGeometry
    let phiSegments = saturnRingGeo.parameters.phiSegments || 0;
    let thetaSegments = saturnRingGeo.parameters.thetaSegments || 0;
    phiSegments = phiSegments !== undefined ? Math.max(1, phiSegments) : 1;
    thetaSegments = thetaSegments !== undefined ? Math.max(3, thetaSegments) : 8;
    for (let c = 0, j = 0; j <= phiSegments; j++) {
      for (let i = 0; i <= thetaSegments; i++) {
        //@ts-ignore
        (uvs[c++] = i / thetaSegments), (uvs[c++] = j / phiSegments);
      }
    }
    const saturnRing = new THREE.Mesh(saturnRingGeo, new THREE.MeshBasicMaterial({ map: saturnRingTexture, side: THREE.DoubleSide }));

    saturnRing.position.x = 949;
    saturnRing.rotation.x = Math.PI / 2;
    let saturnSystem = new THREE.Group();
    saturnSystem.rotation.y = Math.random() * Math.PI * 2;
    saturnSystem.add(saturnMesh, saturnRing);
    //TODO: add uranus

    const uranus = new Planet(9, 1870, 'uranus.jpg');
    const uranusMesh = uranus.getMesh();
    let uranusSystem = new THREE.Group();
    uranusSystem.rotation.y = Math.random() * Math.PI * 2;
    uranusSystem.add(uranusMesh);
    //TODO: add neptune

    const neptune = new Planet(8, 2940, 'neptune.jpg');
    const neptuneMesh = neptune.getMesh();
    let neptuneSystem = new THREE.Group();
    neptuneSystem.rotation.y = Math.random() * Math.PI * 2;
    neptuneSystem.add(neptuneMesh);
    //TODO: add pluto

    solarSystem.add(sun, mercurySystem, venusSystem, earthSystem, marsSystem, jupiterSystem, saturnSystem, uranusSystem, neptuneSystem);
    scene.add(solarSystem);
    const loop = () => {
      const EARTH_YEAR = 2 * Math.PI * (1 / 60) * (1 / 60);
      console.log(EARTH_YEAR);
      sun.rotation.y += 0.004;
      mercuryMesh.rotation.y += 0.0017035775127768313458262350937;
      earthMesh.rotation.y += 0.1;
      venusMesh.rotation.y += 0.00041152263;
      marsMesh.rotation.y += 0.098;
      jupiterMesh.rotation.y += 0.35;
      saturnMesh.rotation.y += 0.33;
      uranusMesh.rotation.y += 0.16;
      neptuneMesh.rotation.y += 0.17;
      mercurySystem.rotation.y += EARTH_YEAR * 4;
      venusSystem.rotation.y += EARTH_YEAR * 2;
      earthSystem.rotation.y += EARTH_YEAR;

      marsSystem.rotation.y += EARTH_YEAR * 0.5;
      jupiterSystem.rotation.y += EARTH_YEAR * 0.2;
      saturnSystem.rotation.y += EARTH_YEAR * 0.1;
      uranusSystem.rotation.y += EARTH_YEAR * 0.05;
      neptuneSystem.rotation.y += EARTH_YEAR * 0.025;
      renderer.render(scene, camera);
      requestAnimationFrame(loop);
    };

    onMounted(() => {
      renderer = new THREE.WebGLRenderer({
        canvas: document.querySelector('canvas') as HTMLCanvasElement,
        antialias: true,
      });
      controls = new OrbitControls(camera, renderer.domElement);
      controls.enableDamping = true;
      renderer.shadowMap.enabled = true;
      renderer.shadowMap.type = THREE.PCFSoftShadowMap;
      updateRenderer();
      updateCamera();
      loop();
      renderer.render(scene, camera);
    });

    return {
      scene,
      camera,
    };
  },
};
</script>
