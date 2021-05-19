terrFac = TerrainFactory:getInstance()



--Creating Player Object
playerID = ObjectManager.Create(Types.player(),"");
player = ObjectManager.GetObject(playerID);
player:setPosition(500.0, 50.0, 500.0);
player:setCollider(4, 4, 4.0);



--Creating SkyBox
skyBoxID = ObjectManager.Create(Types.static(),"content/Models/skybox1.fbx");
skyBox = ObjectManager.GetObject(skyBoxID);
skyBox:setPosition(TerrainSettings.terrainSize/2, 0, TerrainSettings.terrainSize/2);
skyBox:setScale(0.03, 0.03, 0.03);

--Creating Rocks in random places
for i=0,300 do
        x = math.random(0,TerrainSettings.terrainSize);
        z = math.random(0,TerrainSettings.terrainSize);
        val = terrFac:getHeight(x,z)
        asset = ObjectManager.Create(Types.static(),"content/Models/rock.fbx");
        gameObj = ObjectManager.GetObject(asset);
        gameObj:setScale(0.02,0.02,0.02);
        gameObj:setPosition(x,val-2,z);
        gameObj:setCollider(6,6,15);
end

--Creating Zombies in random places
for i=0, 180 do
        x = math.random(0,TerrainSettings.terrainSize);
        z = math.random(0,TerrainSettings.terrainSize);
        val = terrFac:getHeight(x,z)
        asset = ObjectManager.Create(Types.npc(),"content/Models/Zombie/ZombieSmooth.gltf");
        gameObj = ObjectManager.GetObject(asset);
        gameObj:setScale(0.5,0.5,0.5);
        gameObj:setCollider(4,1,25);
        gameObj:setPosition(x,val,z);
end
test = ObjectManager.Create(Types.npc(),"content/Models/Zombie/ZombieSmooth.gltf");
derived = ObjectManager.GetNPC(test);
derived:test();
derived:setPosition(500, 50, 500);
derived:setScale(10, 10, 10);

--Creating Trees in random places
for i=0,300 do
        x = math.random(0,TerrainSettings.terrainSize);
        z = math.random(0,TerrainSettings.terrainSize);
        val = terrFac:getHeight(x,z)
        asset = ObjectManager.Create(Types.static(),"content/Models/tree.fbx");
        gameObj = ObjectManager.GetObject(asset);
        gameObj:setScale(0.01,0.01,0.01);
        gameObj:setCollider(1,1,50);
        gameObj:setPosition(x,val-1,z);
end
