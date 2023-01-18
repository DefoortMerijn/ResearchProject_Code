<template>
  <ArrowLeft class="fixed top-3 right-8 w-6 h-6 stroke-white" @click="CameraReset" />
  <canvas class="w-screen h-screen overflow-hidden"></canvas>
  <div class="fixed">
    <TransitionRoot appear as="template" :show="isOpen">
      <Dialog class="overflow-hidden" :open="isOpen" @close="setIsOpen(false)">
        <TransitionChild enter="transform ease-in-out transition-transform duration-300" enter-from="translate-x-full" enter-to="translate-x-0" leave="transform ease-in-out transition-transform duration-300" leave-from="translate-x-0" leave-to="translate-x-full" as="template">
          <div class="flex flex-col fixed inset-y-0 right-0 w-full max-w-lg bg-slate-800">
            <div class="flex justify-between items-center p-4 shadow">
              <DialogTitle class="text-xl font-bold text-white">Info</DialogTitle>
              <button @click="setIsOpen(false)" class="p-1 rounded-full focus:outline-none focus:ring-2 focus:ring-offset-2">
                <svg xmlns="http://www.w3.org/2000/svg" class="w-6 h-6 stroke-white" fill="none" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" />
                </svg>
              </button>
            </div>

            <div class="flex-1 flex flex-col">
              <div class="flex-1">
                <div class="p-4 overflow-hidden">
                  <div class="mb-2 flex gap-4 bg-black bg-opacity-30 p-3 rounded-lg">
                    <h2 class="text-xl text-white font-semibold">Name:</h2>
                    <p class="text-lg text-neutral-200 font-normal">{{ PlanetInfo.name }}</p>
                  </div>
                  <div class="mb-2 flex gap-4 bg-black bg-opacity-30 p-3 rounded-lg">
                    <h2 class="text-xl text-white font-semibold">Diameter:</h2>
                    <p class="text-lg text-neutral-200 font-normal">{{ PlanetInfo.diameter }}</p>
                  </div>
                  <div class="mb-2 flex gap-4 bg-black bg-opacity-30 p-3 rounded-lg">
                    <h2 class="text-xl text-white font-semibold">Mass:</h2>
                    <p class="text-lg text-neutral-200 font-normal">{{ PlanetInfo.mass }}</p>
                  </div>
                  <div v-if="PlanetInfo.distance_from_Sun != '0 km'" class="mb-2 rounded-lg bg-black bg-opacity-30 p-3 flex gap-4">
                    <h2 class="text-xl text-white font-semibold">Distance from the sun:</h2>
                    <p class="text-lg text-neutral-200 font-normal">{{ PlanetInfo.distance_from_Sun }}</p>
                  </div>
                  <div class="mb-2 flex gap-4 bg-black bg-opacity-30 p-3 rounded-lg">
                    <h2 class="text-xl text-white font-semibold">Surface Area:</h2>
                    <p class="text-lg text-neutral-200 font-normal">{{ PlanetInfo.surface_area }}</p>
                  </div>
                  <div class="mb-2 flex gap-4 bg-black bg-opacity-30 p-3 rounded-lg">
                    <h2 class="text-xl text-white font-semibold">Land Area:</h2>
                    <p class="text-lg text-neutral-200 font-normal">{{ PlanetInfo.land_area }}</p>
                  </div>
                  <div class="mb-2 flex gap-4 bg-black bg-opacity-30 p-3 rounded-lg">
                    <h2 class="text-xl text-white font-semibold">Time per rotation:</h2>
                    <p class="text-lg text-neutral-200 font-normal">{{ PlanetInfo.rotation_time }}</p>
                  </div>
                  <div class="mb-2 flex gap-4 bg-black bg-opacity-30 p-3 rounded-lg">
                    <h2 class="text-xl text-white font-semibold">Gravitational pull:</h2>
                    <p class="text-lg text-neutral-200 font-normal">{{ PlanetInfo.gravity }}</p>
                  </div>
                  <div class="mb-2 bg-black bg-opacity-30 p-3 rounded-lg">
                    <div class="flex gap-4" v-if="PlanetInfo.moons.length === 0">
                      <h2 class="text-xl text-white font-semibold">Moons:</h2>
                      <p class="text-lg text-neutral-200 font-normal">None</p>
                    </div>
                    <div v-else>
                      <h2 class="text-xl text-white font-semibold mb-2">Moons:</h2>

                      <ul class="bg-black bg-opacity-30 p-3 rounded-lg">
                        <li class="text-lg text-neutral-200 font-normal" v-for="moon of PlanetInfo.moons">{{ moon }}</li>
                      </ul>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </TransitionChild>
      </Dialog>
    </TransitionRoot>
  </div>
</template>

<script lang="ts">
import * as THREE from 'three';
import { computed, onMounted, watch, ref } from 'vue';
import { useWindowSize } from '@vueuse/core';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import { ArrowLeft } from 'lucide-vue-next';
import CreatePlanet from './components/CreatePlanet';
import { Planet } from './interfaces/Planet';
import * as s from './assets/json/SolarSystem.json';

import { TransitionRoot, TransitionChild, Dialog, DialogPanel, DialogTitle } from '@headlessui/vue';
export default {
  components: {
    TransitionRoot,
    TransitionChild,
    Dialog,
    DialogPanel,
    DialogTitle,
    ArrowLeft,
  },
  setup() {
    const scene = new THREE.Scene();
    scene.background = new THREE.TextureLoader().load('PlanetTextures/space.jpg');
    const { width, height } = useWindowSize();
    let renderer: THREE.WebGLRenderer;
    let camera: THREE.PerspectiveCamera;
    const isOpen = ref(false);
    let controls: OrbitControls;
    const aspectRatio = computed(() => width.value / height.value);
    let focused = false;
    let rotSpeed = 0;
    let PlanetInfo: Planet = {
      name: '',
      diameter: '',
      mass: '',
      distance_from_Sun: '',
      surface_area: '',
      land_area: '',
      rotation_time: '',
      gravity: '',
      moons: [],
    };

    function setIsOpen(state: boolean) {
      isOpen.value = state;

      if (state == false) {
        CameraReset();
      }
    }
    let position = new THREE.Vector3();
    let planet: THREE.Object3D;

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
      scene.updateMatrixWorld(true);
      position.setFromMatrixPosition(planet.matrixWorld);
      camera.lookAt(position);
      camera.position.set(position.x, position.y, position.z);
    }

    function AddPlanetInfo(planet: THREE.Object3D) {
      let planetName = planet.name;

      for (let p of s.solarsystem) {
        console.log();

        if (planetName === p.name) {
          PlanetInfo.name = p.name;
          PlanetInfo.diameter = p.diameter;
          PlanetInfo.mass = p.mass;
          PlanetInfo.distance_from_Sun = p.distance_from_Sun;
          PlanetInfo.surface_area = p.surface_area;
          PlanetInfo.land_area = p.land_area;
          PlanetInfo.rotation_time = p.rotation_time;
          PlanetInfo.gravity = p.gravity;
          PlanetInfo.moons = p.moons!;
          console.log(PlanetInfo);
        }
      }
    }

    function CameraReset() {
      marsMesh.remove(camera);
      earthSystem.remove(camera);
      venusMesh.remove(camera);
      mercuryMesh.remove(camera);
      jupiterMesh.remove(camera);
      saturnMesh.remove(camera);
      uranusMesh.remove(camera);
      neptuneMesh.remove(camera);
      sun.remove(camera);
      scene.add(camera);
      focused = false;
      controls.reset();
      isOpen.value = false;
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
    const sunTexture = new THREE.TextureLoader().load('PlanetTextures/sun.jpg');
    const sun = new THREE.Mesh(new THREE.SphereGeometry(60, 90, 90), new THREE.MeshBasicMaterial({ map: sunTexture }));
    sun.name = 'Sun';
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
    const mercury = new CreatePlanet(2, 95, 'PlanetTextures/mercury.jpg');
    const mercuryMesh = mercury.getMesh();
    mercuryMesh.name = 'Mercury';
    let mercurySystem = new THREE.Group();
    mercurySystem.rotation.y = Math.random() * Math.PI * 2;
    mercurySystem.add(mercuryMesh);
    //Venus creation
    const venus = new CreatePlanet(5.8, 127, 'PlanetTextures/venus.jpg');
    const venusMesh = venus.getMesh();
    venusMesh.name = 'Venus';
    let venusSystem = new THREE.Group();
    venusSystem.rotation.y = Math.random() * Math.PI * 2;
    venusSystem.add(venusMesh);
    //Earth creation
    const earth = new CreatePlanet(6, 153, 'PlanetTextures/earth.jpg');
    const earthMesh = earth.getMesh();
    earthMesh.name = 'Earth';
    let earthSystem = new THREE.Group();

    earthSystem.rotation.y = Math.random() * Math.PI * 2;
    earthSystem.add(earthMesh);

    //Mars creation
    const mars = new CreatePlanet(3, 202, 'PlanetTextures/mars.jpg');
    const marsMesh = mars.getMesh();
    marsMesh.name = 'Mars';
    let marsSystem = new THREE.Group();
    marsSystem.rotation.y = Math.random() * Math.PI * 2;
    marsSystem.add(marsMesh);

    //Jupiter creation
    const jupiter = new CreatePlanet(20, 544, 'PlanetTextures/jupiter.jpg');
    const jupiterMesh = jupiter.getMesh();
    jupiterMesh.name = 'Jupiter';
    let jupiterSystem = new THREE.Group();
    jupiterSystem.rotation.y = Math.random() * Math.PI * 2;
    jupiterSystem.add(jupiterMesh);

    //Saturn + Ring creation
    const saturn = new CreatePlanet(17, 949, 'PlanetTextures/saturn.jpg');
    const saturnMesh = saturn.getMesh();
    saturnMesh.name = 'Saturn';
    const saturnRingTexture = new THREE.TextureLoader().load('PlanetTextures/rings.jpg', function (texture) {
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
    const saturnRing = new THREE.Mesh(saturnRingGeo, new THREE.MeshBasicMaterial({ map: saturnRingTexture, side: THREE.DoubleSide, shadowSide: THREE.DoubleSide, transparent: true, opacity: 0.5 }));
    saturnRing.position.x = saturnMesh.position.x;
    saturnRing.rotation.x = 90;
    saturnRing.receiveShadow = true;
    saturnRing.castShadow = true;

    let saturnSystem = new THREE.Group();
    saturnSystem.rotation.y = Math.random() * Math.PI * 2;
    saturnSystem.add(saturnMesh, saturnRing);
    //Uranus creation
    const uranus = new CreatePlanet(9, 1870, 'PlanetTextures/uranus.jpg');
    const uranusMesh = uranus.getMesh();
    uranusMesh.name = 'Uranus';
    let uranusSystem = new THREE.Group();
    uranusSystem.rotation.y = Math.random() * Math.PI * 2;
    uranusSystem.add(uranusMesh);

    //Neptune creation
    const neptune = new CreatePlanet(8, 2940, 'PlanetTextures/neptune.jpg');
    const neptuneMesh = neptune.getMesh();
    neptuneMesh.name = 'Neptune';
    let neptuneSystem = new THREE.Group();
    neptuneSystem.rotation.y = Math.random() * Math.PI * 2;
    neptuneSystem.add(neptuneMesh);

    //add all planets to solarsystem
    solarSystem.add(sun, mercurySystem, venusSystem, earthSystem, marsSystem, jupiterSystem, saturnSystem, uranusSystem, neptuneSystem);
    scene.add(solarSystem);

    //Animations
    const animate = () => {
      const EARTH_YEAR = Math.PI * (1 / 60) * (1 / 60);
      sun.rotation.y += 0.004;
      mercuryMesh.rotation.y += 0.004;
      venusMesh.rotation.y += 0.004;
      earthMesh.rotation.y += 0.004;
      marsMesh.rotation.y += 0.004;
      jupiterMesh.rotation.y += 0.004;
      saturnMesh.rotation.y += 0.004;
      uranusMesh.rotation.y += 0.004;
      neptuneMesh.rotation.y += 0.004;

      mercurySystem.rotation.y += EARTH_YEAR * 4;
      venusSystem.rotation.y += EARTH_YEAR * 2;
      earthSystem.rotation.y += EARTH_YEAR;

      marsSystem.rotation.y += EARTH_YEAR * 0.5;
      jupiterSystem.rotation.y += EARTH_YEAR * 0.2;
      saturnSystem.rotation.y += EARTH_YEAR * 0.1;
      uranusSystem.rotation.y += EARTH_YEAR * 0.05;
      neptuneSystem.rotation.y += EARTH_YEAR * 0.025;
      if (focused) {
        //TODO fix camera rotation around selectial body
        position.setFromMatrixPosition(planet.matrixWorld);
        console.log(camera.position);

        camera.position.x = position.x - 20 * Math.cos(0) - position.z - 20 * Math.sin(1);
        camera.position.z = position.x - 20 * Math.sin(1) + position.z - 20 * Math.cos(0);
        camera.position.y = 0;

        camera.lookAt(position);
        updateCamera();
        updateRenderer();
      }

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
      lightSetup();
      animate();
      sun.addEventListener('click', () => {
        console.log('clicked');
      });
      // renderer.render(scene, camera);
    });

    //When a planet is clicked, the camera will move to the planet
    window.addEventListener('click', (event) => {
      pointer.x = (event.clientX / window.innerWidth) * 2 - 1;
      pointer.y = -(event.clientY / window.innerHeight) * 2 + 1;
      raycaster.setFromCamera(pointer, camera);
      const intersects = raycaster.intersectObjects(scene.children, true);
      if (intersects.length > 0) {
        planet = intersects[0].object;
        focused = true;
        setIsOpen(true);
        trackPlanet(planet);
        AddPlanetInfo(planet);
      }
    });

    return {
      scene,
      camera,
      CameraReset,
      isOpen,
      setIsOpen,
      PlanetInfo,
      AddPlanetInfo,
    };
  },
};
</script>
