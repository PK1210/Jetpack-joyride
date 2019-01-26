#include "engine.h"

Engine::Engine(int level) {
    this->base = Platform(-8, FLOOR, COLOR_GREEN);
    this->roof = Platform(-8, CEILING + 2.0f, COLOR_GREEN);
    this->player = Player(-7, FLOOR + 2.5f, COLOR_ORANGE);
    this->score = 0;
    this->score_display = Number_display(FRAME - 2.0f, CEILING + 1.0f, this->score);
    this->life_display = Number_display(FRAME + 2.0f, CEILING + 1.0f, this->player.lives);
    this->counter = 0;
}

void Engine::draw(glm::mat4 VP) {
    this->roof.draw(VP);
    this->base.draw(VP);

    this->player.draw(VP);
    this->score_display.draw(VP);
    this->life_display.draw(VP);

    draw_template(this->magnets, VP);
    draw_template(this->coins, VP);
    draw_template(this->boomerangs, VP);
    draw_template(this->hearts, VP);
    draw_template(this->missiles, VP);
    draw_template(this->dragons, VP);
    draw_template(this->firebeams, VP);
    draw_template(this->lasers, VP);
    draw_template(this->waterballs, VP);
    draw_template(this->shields, VP);
    draw_template(this->iceballs, VP);
    draw_template(this->rings, VP);
}

void Engine::tick() {
    this->counter++;
    this->player.tick();
    this->score_display = Number_display(FRAME - 3.0f, CEILING + 1.0f, this->score);
    this->life_display = Number_display(FRAME + 3.0f, CEILING + 1.0f, this->player.lives);

    // Produce stuffs
    if(counter%827 == 7)
        this->magnets.push_back(Magnet(FRAME + rand()%ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT));
    if((counter%67) == 7)
        this->coins.push_back(Coin(FRAME + ARENA_WIDTH + rand()%ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT, 0));
    if((counter%127) == 7)
        this->coins.push_back(Coin(FRAME + ARENA_WIDTH + rand()%ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT, 1));
    if((counter%259) == 7)
        this->coins.push_back(Coin(FRAME + ARENA_WIDTH + rand()%ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT, 2));
    if(counter%401 == 7)
        this->boomerangs.push_back(Boomerang(FRAME + ARENA_WIDTH + rand()%ARENA_WIDTH/2, SAFE_FLOOR + ARENA_HEIGHT, COLOR_ORANGE));
    if(counter%629 == 7)
        this->boomerangs.push_back(Boomerang(FRAME + ARENA_WIDTH + rand()%ARENA_WIDTH/2, SAFE_FLOOR + rand()%ARENA_HEIGHT, COLOR_ORANGE));
    if((counter%512) == 7)
        this->hearts.push_back(Heart(FRAME + ARENA_WIDTH + rand()%ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT));
    if((counter%659) == 7)
        this->missiles.push_back(Missile(FRAME + ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT, COLOR_RED));
    if((counter%817) == 7)
        this->dragons.push_back(Dragon(FRAME + ARENA_WIDTH + rand()%ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT));
    if((counter%1507) == 7)
        this->firebeams.push_back(Firebeam(FRAME + ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT, COLOR_RED));
    if((counter%209) == 7)
        this->lasers.push_back(Laser(FRAME + ARENA_WIDTH/2 + rand()%(ARENA_WIDTH - 2), SAFE_FLOOR + rand()%ARENA_HEIGHT, M_PI/(rand()%24)));
    if((counter%611) == 7)
        this->shields.push_back(Shield(FRAME + ARENA_WIDTH/2 + rand()%ARENA_WIDTH, SAFE_FLOOR + rand()%ARENA_HEIGHT));
    if((counter%1611) == 7)
        this->rings.push_back(Ring(FRAME + ARENA_WIDTH/2 + rand()%ARENA_WIDTH, SAFE_FLOOR + 1 + rand()%(ARENA_HEIGHT-2)));

    // Tick other stuff
    for (auto it = this->magnets.begin(); it != this->magnets.end(); it++) {
        it->tick(player.position.x, player.position.y);
        player.magnetic_motion(it->position.x, it->position.y);
    }
    for (auto it = this->coins.begin(); it != this->coins.end(); it++)
        it->tick();
    for (auto it = this->boomerangs.begin(); it != this->boomerangs.end(); it++)
        it->tick();
    for (auto it = this->hearts.begin(); it != this->hearts.end(); it++)
        it->tick();
    for (auto it = this->missiles.begin(); it != this->missiles.end(); it++)
        it->tick();
    for (auto it = this->dragons.begin(); it != this->dragons.end(); it++) {
        it->tick(player.position.x, player.position.y);
        if(it->shoot(counter)) {
            this->iceballs.push_back(Iceball(it->position.x, it->position.y, this->player.position.x, this->player.position.y));
        }
    }
    for (auto it = this->firebeams.begin(); it != this->firebeams.end(); it++)
        it->tick();
    for (auto it = this->lasers.begin(); it != this->lasers.end(); it++)
        it->tick();
    for (auto it = this->waterballs.begin(); it != this->waterballs.end(); it++)
        it->tick();
    for (auto it = this->shields.begin(); it != this->shields.end(); it++)
        it->tick();
    for (auto it = this->iceballs.begin(); it != this->iceballs.end(); it++)
        it->tick();
}

void Engine::tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);

    if (left) {
        this->player.move(0);
    }
    if (right) {
      this->player.move(1);
    }
    if (space || up) {
      this->player.tickUp();
    }
    if (down) {
        if(this->player.shoot(this->counter)) {
            this->waterballs.push_back(Waterball(this->player.position.x + 0.1f, this->player.position.y - 0.1f));
            this->waterballs.push_back(Waterball(this->player.position.x, this->player.position.y));
            this->waterballs.push_back(Waterball(this->player.position.x - 0.1f, this->player.position.y + 0.1f));
        }
    }
}

int Engine::get_life() {
        return this->player.lives;
}

void Engine::collider() {
    // Detect coin capture
    for (auto it = this->coins.begin(); it != this->coins.end(); it++) {
        if(detect_collision(player.box(),it->box()))
        {
            it->position.y -= 100;
            score++;
            score+=it->type;
        }
    }

    // Detect collision with boomerang
    for (auto it = this->boomerangs.begin(); it != this->boomerangs.end(); it++) {
        if(detect_collision(player.box(), it->box())){
            player.die();
            it->position.y = -100;
            it->speed_x = -10000;
        }
    }
    // Detect heart capture
    for (auto it = this->hearts.begin(); it != this->hearts.end(); it++) {
        if(detect_collision(player.box(),it->box()))
        {
            it->position.y -= 100;
            player.get_life();
        }
    }
    // Detect collision with missile
    for (auto it = this->missiles.begin(); it != this->missiles.end(); it++) {
        if(detect_collision(player.box(), it->box())){
            player.die();
            it->position.y = -100;
        }
    }
    // Detect collision with missile
    for (auto it = this->dragons.begin(); it != this->dragons.end(); it++) {
        if(detect_collision(player.box(), it->box())){
            player.die();
            it->position.y = -100;
        }
    }

    // Detect collision from firebeam
    for (auto it = this->firebeams.begin(); it != this->firebeams.end(); it++) {
        if(detect_collision(player.box(), it->box())){
            player.die();
            it->position.y = -100;
        }
    }

    // Detect collision from Laser
    for (auto it = this->lasers.begin(); it != this->lasers.end(); it++) {
        if(it->collision(player.box())){
            player.die();
            it->position.y = -100;
        }
    }

    // Detect collision from waterballs
    for (auto jt = this->waterballs.begin(); jt != this->waterballs.end(); jt++) {
        for (auto it = this->lasers.begin(); it != this->lasers.end(); it++) {
            if(it->collision(jt->box())){
                jt->position.y = GRAVE;
                it->position.y = -100;
            }
        }
    }

    // Detect collision from shield
    for (auto it = this->shields.begin(); it != this->shields.end(); it++) {
        if(detect_collision(player.box(), it->box())){
            player.shield = true;
            it->position.y = -100;
        }
    }

    // Check death due to iceballs
    for (auto it = this->iceballs.begin(); it != this->iceballs.end(); it++) {
        if(detect_collision(player.box(),it->box())){
            it->position.y = GRAVE;
            player.die();
        }
    }
}
