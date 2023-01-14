<template>
  <ArrowLeft class="fixed top-3 right-8 w-6 h-6 stroke-white" @click="CameraReset" />
  <canvas class="w-screen h-screen overflow-hidden"></canvas>
</template>

<script lang="ts">
import * as THREE from 'three';
import { computed, onMounted, watch } from 'vue';
import { useWindowSize } from '@vueuse/core';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import { ArrowLeft } from 'lucide-vue-next';
import Planet from './components/Planet';
import { InteractionManager } from 'three.interactive';

export default {
  components: {
    ArrowLeft,
  },
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

    //track current position of a planet
    function trackPlanet(planet: THREE.Object3D) {
      console.log(planet);

      scene.updateMatrixWorld(true);
      var position = new THREE.Vector3();
      position.setFromMatrixPosition(planet.matrixWorld);
      alert(position.x + ',' + position.y + ',' + position.z);
      camera.position.set(position.x, position.y, position.z + 100);
      camera.lookAt(planet.position);
    }

    function CameraReset() {
      controls.reset();
    }

    watch(aspectRatio, () => {
      updateCamera();
      updateRenderer();
      console.log(earthSystem.rotation);
    });

    camera = new THREE.PerspectiveCamera(15, aspectRatio.value, 0.1, 100000);
    camera.position.y = 1000;

    const pointer = new THREE.Vector2();
    const raycaster = new THREE.Raycaster();
    const sunTexture = new THREE.TextureLoader().load('sun.jpg');
    const sun = new THREE.Mesh(new THREE.SphereGeometry(60, 90, 90), new THREE.MeshBasicMaterial({ map: sunTexture }));
    sun.name = 'sun';
    const solarSystem = new THREE.Group();

    const light1 = new THREE.PointLight(0xffffff, 1.2, 10000);
    const lightSetup = () => {
      light1.position.set(0, 0, 0);
      light1.rotateY(Math.PI / 2);
      light1.castShadow = true; // default false

      scene.add(light1);
      // const helper = new THREE.CameraHelper(light1.shadow.camera);
      // light1.add(helper);
    };

    /*PLANETS*/
    // Mercury creation
    const mercury = new Planet(2, 95, 'mercury.jpg');
    const mercuryMesh = mercury.getMesh();
    mercuryMesh.name = 'mercury';
    let mercurySystem = new THREE.Group();
    mercurySystem.rotation.y = Math.random() * Math.PI * 2;
    mercurySystem.add(mercuryMesh);
    //Venus creation
    const venus = new Planet(5.8, 127, 'venus.jpg');
    const venusMesh = venus.getMesh();
    venusMesh.name = 'venus';
    let venusSystem = new THREE.Group();
    venusSystem.rotation.y = Math.random() * Math.PI * 2;
    venusSystem.add(venusMesh);
    //Earth creation
    const earth = new Planet(6, 153, 'earth.jpg');
    const earthMesh = earth.getMesh();
    earthMesh.name = 'earth';
    let earthSystem = new THREE.Group();
    earthSystem.rotation.y = Math.random() * Math.PI * 2;
    earthSystem.add(earthMesh);

    //Mars creation
    const mars = new Planet(3, 202, 'mars.jpg');
    const marsMesh = mars.getMesh();
    marsMesh.name = 'mars';
    let marsSystem = new THREE.Group();
    marsSystem.rotation.y = Math.random() * Math.PI * 2;
    marsSystem.add(marsMesh);

    //Jupiter creation
    const jupiter = new Planet(20, 544, 'jupiter.jpg');
    const jupiterMesh = jupiter.getMesh();
    jupiterMesh.name = 'jupiter';
    let jupiterSystem = new THREE.Group();
    jupiterSystem.rotation.y = Math.random() * Math.PI * 2;
    jupiterSystem.add(jupiterMesh);

    //Saturn + Ring creation
    const saturn = new Planet(17, 949, 'saturn.jpg');
    const saturnMesh = saturn.getMesh();
    saturnMesh.name = 'saturn';
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
    //Uranus creation
    const uranus = new Planet(9, 1870, 'uranus.jpg');
    const uranusMesh = uranus.getMesh();
    uranusMesh.name = 'uranus';
    let uranusSystem = new THREE.Group();
    uranusSystem.rotation.y = Math.random() * Math.PI * 2;
    uranusSystem.add(uranusMesh);

    //Neptune creation
    const neptune = new Planet(8, 2940, 'neptune.jpg');
    const neptuneMesh = neptune.getMesh();
    neptuneMesh.name = 'neptune';
    let neptuneSystem = new THREE.Group();
    neptuneSystem.rotation.y = Math.random() * Math.PI * 2;
    neptuneSystem.add(neptuneMesh);

    //add all planets to solarsystem
    solarSystem.add(sun, mercurySystem, venusSystem, earthSystem, marsSystem, jupiterSystem, saturnSystem, uranusSystem, neptuneSystem);
    scene.add(solarSystem);

    //Animations
    const animate = () => {
      const EARTH_YEAR = 2 * Math.PI * (1 / 60) * (1 / 60);
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
      requestAnimationFrame(animate);
    };

    // Render the scene
    onMounted(() => {
      renderer = new THREE.WebGLRenderer({
        canvas: document.querySelector('canvas') as HTMLCanvasElement,
        antialias: true,
      });
      controls = new OrbitControls(camera, renderer.domElement);
      renderer.shadowMap.enabled = true;
      renderer.shadowMap.type = THREE.PCFSoftShadowMap;
      updateRenderer();
      updateCamera();
      const interactionManager = new InteractionManager(renderer, camera, document.querySelector('canvas') as HTMLCanvasElement, false);
      lightSetup();
      animate();
      sun.addEventListener('click', () => {
        console.log('clicked');
      });
      renderer.render(scene, camera);
    });

    //When a planet is clicked, the camera will move to the planet
    window.addEventListener('click', (event) => {
      console.log(event.clientX, event.clientY);

      pointer.x = (event.clientX / window.innerWidth) * 2 - 1;
      pointer.y = -(event.clientY / window.innerHeight) * 2 + 1;
      raycaster.setFromCamera(pointer, camera);
      const intersects = raycaster.intersectObjects(scene.children, true);
      if (intersects.length > 0) {
        const planet = intersects[0].object;

        const planetName = planet.name;

        trackPlanet(planet);
        console.log(planetName);
      }
    });

    return {
      scene,
      camera,
      CameraReset,
    };
  },
};
</script>
