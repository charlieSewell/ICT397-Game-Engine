
ammoNumbers = {};
ammoNumbers[0] = "content/Textures/0.png"
ammoNumbers[1] = "content/Textures/1.png"
ammoNumbers[2] = "content/Textures/2.png"
ammoNumbers[3] = "content/Textures/3.png"
ammoNumbers[4] = "content/Textures/4.png"
ammoNumbers[5] = "content/Textures/5.png"
ammoNumbers[6] = "content/Textures/6.png"
ammoNumbers[7] = "content/Textures/7.png"
ammoNumbers[8] = "content/Textures/8.png"
ammoNumbers[9] = "content/Textures/9.png"

if (player.gun.reserveAmmo >= 0)
then
    remainder = math.fmod(player.gun.ammo, 10);
    uiManager:setTexture("ammoOne", ammoNumbers[(player.gun.ammo - remainder) / 10]);
    uiManager:setTexture("ammoTwo", ammoNumbers[remainder]);

    rRemainder = math.fmod(player.gun.reserveAmmo, 10);

    if(player.gun.reserveAmmo >= 100)
    then
         uiManager:setTexture("reserveOne", ammoNumbers[1]);
         uiManager:setTexture("reserveTwo", ammoNumbers[(player.gun.reserveAmmo - 100 - rRemainder) / 10]);
    else
        uiManager:setTexture("reserveOne", ammoNumbers[0]);
        uiManager:setTexture("reserveTwo", ammoNumbers[(player.gun.reserveAmmo - rRemainder) / 10]);
    end
    uiManager:setTexture("reserveThree", ammoNumbers[rRemainder]);
end

--player.health = 90; works