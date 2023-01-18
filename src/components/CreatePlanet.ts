import * as THREE from 'three';

export default class CreatePlanet {
  public radius: any;
  public positionX: any;
  public textureFile: any;
  public mesh: any;

  constructor(radius: any, positionX: any, textureFile: any) {
    this.radius = radius;
    this.positionX = positionX;
    this.textureFile = textureFile;
  }

  getMesh() {
    if (this.mesh === undefined || this.mesh === null) {
      const geometry = new THREE.SphereGeometry(this.radius, 32, 32);
      const texture = new THREE.TextureLoader().load(this.textureFile);
      const material = new THREE.MeshLambertMaterial({ map: texture });
      this.mesh = new THREE.Mesh(geometry, material);
      this.mesh.position.x += this.positionX;
    }
    return this.mesh;
  }
}
