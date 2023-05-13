#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

int main()
{

    // D�finir le score initial
    int score = 0;

    // Cr�er une fen�tre de jeu
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sky Frenzy");

    // Charger les textures
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("background1.jpg");

    // Charger la texture 2eme fond d'�cran
    sf::Texture background2Texture;
    background2Texture.loadFromFile("background2.png");

    // Charger la texture 3me fond d'�cran
    sf::Texture background3Texture;
    background3Texture.loadFromFile("background3.png");

    // Charger la texture de la fenetre du fin de jeu
    sf::Texture endBackgroundTexture;
    endBackgroundTexture.loadFromFile("endBackground.png");

    //Charger et remplacer le score 
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text scoreText("Score: " + std::to_string(score), font, 30);
    scoreText.setPosition(10.f, 10.f);

    //Charger la cartouche 
    sf::RectangleShape bullet(sf::Vector2f(10.f, 5.f));

    //Charger la texture du bomb et de l'avion 
    sf::Texture planeTexture;
    planeTexture.loadFromFile("plane.png");

    sf::Texture bombTexture;
    bombTexture.loadFromFile("bomb.png");

    // Cr�er les sprites pour les fond et l'avion
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Sprite planeSprite(planeTexture);
    sf::Sprite endBackgroundSprite(endBackgroundTexture);


    sf::SoundBuffer buffer;
    buffer.loadFromFile("victory.wav");
    sf::Sound Yeah;
    Yeah.setBuffer(buffer);

    // Positionner les sprites
    backgroundSprite.setPosition(0, 0);
    planeSprite.setPosition(400, 300);



    // Charger la musique
    sf::Music music;
    music.openFromFile("game.wav");
    music.setLoop(true);
    music.play();

    sf::Music crashMusic;
    crashMusic.openFromFile("game over.wav");
    bool hasCrashed = false;

    sf::Music VictoryMusic;
    VictoryMusic.openFromFile("victory.wav");// say yeah!

    sf::Music endGameMusic;
    endGameMusic.openFromFile("Level Complete.wav");

    // initialiser window2 � null pour detecter sa fermeture dans la boucle principale
    sf::RenderWindow window2;

    // initialiser le g�n�rateur al�atoire
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(50, 750);
    std::uniform_int_distribution<> disY(50, 550);

    // stocker les bombes g�n�r�es
    std::vector<sf::Sprite> bombs;

    // D�finir la vitesse de l'avion
    float planeSpeed = 0.00f;

    // Position initiale de la cartouche
    sf::Vector2f bulletPos(-10.f, -10.f);

    // Boucle principale du jeu
    while ((window.isOpen()) && (!hasCrashed)) //La 2e condition et pour ne pas r�p�ter la cr�ation de la fen�tre de Game over � chaque it�ration de la boucle principale 
    {
        // G�rer les �v�nements de la fen�tre
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    // Tirer une cartouche
                    if (bulletPos.x < 0.f) {
                        bulletPos.x = planeSprite.getPosition().x + planeTexture.getSize().x;
                        bulletPos.y = planeSprite.getPosition().y + planeTexture.getSize().y / 2;
                    }
                }
            }

        }


        // G�n�rer une nouvelle bombe si la pr�c�dente a �t� d�truite  Et v�rifiez qu'elle ne se co�ncide pas avec l'avion 
        if (bombs.empty()) {
            sf::Sprite bombSprite(bombTexture);
            sf::FloatRect planeBounds = planeSprite.getGlobalBounds();
            bool isOnPlane = false;
            do {
                bombSprite.setPosition(disX(gen), disY(gen));
                sf::FloatRect bombBounds = bombSprite.getGlobalBounds();
                if (planeBounds.intersects(bombBounds)) {
                    isOnPlane = true;
                }
            } while (isOnPlane);
            bombs.push_back(bombSprite);
            //stage 2
            if (score >= 5) {
                sf::Sprite bombSprite1(bombTexture);
                sf::FloatRect planeBounds = planeSprite.getGlobalBounds();
                bool isOnPlane = false;
                do {
                    bombSprite1.setPosition(disX(gen) - 50, disY(gen));
                    sf::FloatRect bombBounds = bombSprite1.getGlobalBounds();
                    if (planeBounds.intersects(bombBounds)) {
                        isOnPlane = true;
                    }
                } while (isOnPlane);
                bombs.push_back(bombSprite1);
            }
            //stage3
            if (score >= 10) {
                for (int i = 0;i < 2;i++)
                {
                    sf::Sprite bombSprite1(bombTexture);
                    sf::FloatRect planeBounds = planeSprite.getGlobalBounds();
                    bool isOnPlane = false;
                    do {
                        bombSprite1.setPosition(disX(gen) - 50, disY(gen));
                        sf::FloatRect bombBounds = bombSprite1.getGlobalBounds();
                        if (planeBounds.intersects(bombBounds)) {
                            isOnPlane = true;
                        }
                    } while (isOnPlane);
                    bombs.push_back(bombSprite1);
                }
            }
        }




        // D�placer la cartouche
        if (bulletPos.x >= 0.f) {
            bulletPos.x += 5.f;
            if (bulletPos.x > window.getSize().x) {
                bulletPos.x = -10.f;
                bulletPos.y = -10.f;
            }
        }

        // D�placer l'avion en fonction des contr�les du joueur
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            planeSpeed -= 0.01f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            planeSpeed += 0.01f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            planeSprite.rotate(-1.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            planeSprite.rotate(1.f);
        }
        // D�placer l'avion en fonction de sa vitesse
        planeSprite.move(cos(planeSprite.getRotation() * 3.14 / 180) * planeSpeed, sin(planeSprite.getRotation() * 3.14 / 180) * planeSpeed);


        // V�rifier si une bombe a touch� l'avion
        for (auto& bomb : bombs)
        {
            if (bomb.getGlobalBounds().intersects(planeSprite.getGlobalBounds()))
            {
                planeSpeed = 0.00f; //Arr�ter l'avion 
                hasCrashed = true;
                music.stop();

            }
            if (hasCrashed)
            {     //Cr�er la fen�tre de Game over 
                window2.create(sf::VideoMode(600, 300), "Game Over");

                //Utiliser le fond valuoldcaps.ttf
                sf::Font font;
                font.loadFromFile("valuoldcaps.ttf");
                sf::Text text("GAME OVER", font, 50);

                //Centrer le texte � la fen�tre Game over 
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                text.setPosition(window2.getSize().x / 2.0f, window2.getSize().y / 2.0f);

                window2.draw(text);
                window2.display();
                crashMusic.play();
                while (crashMusic.getStatus() == sf::SoundSource::Playing)
                {   //Profitez cette boucle pour attendre la fin de la musique de crash et � la fois pour d�tecter la fermeture de la fen�tre Game over
                    sf::Event event;
                    while (window2.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                        {
                            window2.close();
                        }
                    }
                }
            }
        }

        // V�rifier si la cartouche a touch� une bombe
        for (auto it = bombs.begin(); it != bombs.end(); )
        {
            if (it->getGlobalBounds().contains(bulletPos))
            {
                score++;
                it = bombs.erase(it);
                bulletPos.x = -10.f;
                bulletPos.y = -10.f;
            }
            else {
                ++it;
            }
        }


        if (score == 4 || score == 9)
        {
            Yeah.play();
        }

        // stage 2
        if (score >= 5) {
            // Utiliser le fond d'�cran du stage 2
            window.clear();
            sf::Sprite Background2Sprite(background2Texture);
            backgroundSprite.setTexture(background2Texture);
        }
        // stage 3
        if (score >= 10) {
            // Utiliser le fond d'�cran du stage 3
            window.clear();
            sf::Sprite Background3Sprite(background3Texture);
            backgroundSprite.setTexture(background3Texture);

            //couleur du cartouche modifi�e selon le couleur du background
            bullet.setPosition(bulletPos);
            bullet.setFillColor(sf::Color::White);//changer le couleur du cartouche en blanc
            window.draw(bullet);

            //score modifi� selon le couleur du background           
            scoreText.setFillColor(sf::Color::White);//changer le couleur du text en blanc
            window.draw(scoreText);

        }


        // Effacer l'�cran
        window.clear();

        // Dessiner le fond et l'avion
        window.draw(backgroundSprite);
        window.draw(planeSprite);

        // Dessiner les bombes
        for (auto& bomb : bombs)
        {
            window.draw(bomb);
        }

        // Dessiner la cartouche
        sf::RectangleShape bullet(sf::Vector2f(10.f, 5.f));
        bullet.setPosition(bulletPos);
        window.draw(bullet);
       




        // Afficher le score
        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text scoreText("Score: " + std::to_string(score), font, 30);
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);
       





        //Cr�er la fen�tre du fin du jeu 

        if (score == 19)
        {
            music.stop();
            // Fermer la fen�tre de jeu
            window.close();
            endGameMusic.play();
            // Cr�er la fen�tre de fin de jeu
            sf::RenderWindow endGame(sf::VideoMode(900, 600), "endGame!");

            // afficher le score final

            scoreText.setFillColor(sf::Color::Black);
            scoreText.setPosition(endGame.getSize().x / 2 - scoreText.getGlobalBounds().width / 2, endGame.getSize().y / 2 - scoreText.getGlobalBounds().height / 2);

            // Cr�er le bouton "Restart"
            sf::RectangleShape restartButton(sf::Vector2f(100.f, 50.f));
            restartButton.setFillColor(sf::Color::Green);
            restartButton.setOutlineThickness(2.f);
            restartButton.setOutlineColor(sf::Color::Black);
            sf::Text restartText;
            restartText.setFont(font);
            restartText.setCharacterSize(20);
            restartText.setFillColor(sf::Color::Black);
            restartText.setString("Restart");
            restartText.setPosition(restartButton.getPosition().x + restartButton.getSize().x / 2 - restartText.getGlobalBounds().width / 2 + 340.f, endGame.getSize().y - restartButton.getSize().y - restartText.getGlobalBounds().height + 10.f);
            // Cr�er le bouton "Exit"
            sf::RectangleShape exitButton(sf::Vector2f(100.f, 50.f));
            exitButton.setFillColor(sf::Color::Red);
            exitButton.setOutlineThickness(2.f);
            exitButton.setOutlineColor(sf::Color::Black);
            sf::Text exitText;
            exitText.setFont(font);
            exitText.setCharacterSize(20);
            exitText.setFillColor(sf::Color::Black);
            exitText.setString("Exit");
            exitText.setPosition(restartText.getPosition().x + restartText.getGlobalBounds().width + 70.f, restartText.getPosition().y);
            // Positionner les boutons
            restartButton.setPosition(endGame.getSize().x / 2 - restartButton.getSize().x - 10.f, endGame.getSize().y - restartButton.getSize().y - 20.f);
            exitButton.setPosition(endGame.getSize().x / 2 + 10.f, endGame.getSize().y - exitButton.getSize().y - 20.f);

            // Boucle d'affichage de la fen�tre de fin de jeu
            while (endGame.isOpen())
            {
                // G�rer les �v�nements de la fen�tre
                sf::Event event;
                while (endGame.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        endGame.close();
                    }

                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        // V�rifier si le joueur a cliqu� sur le bouton "Restart"
                        if (restartButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
                        {
                            endGame.close();
                            main();
                        }

                        // V�rifier si le joueur a cliqu� sur le bouton "Exit"
                        if (exitButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
                        {
                            endGame.close();
                        }
                    }
                }
                // ajuster la photo a la fenetre et dessiner le background  
                endBackgroundSprite.setScale(900.f / endBackgroundTexture.getSize().x, 600.f / endBackgroundTexture.getSize().y);
                endGame.draw(endBackgroundSprite);

                // Dessiner le texte du score final 
                endGame.draw(scoreText);

                // Dessiner les boutons "Restart" et "Exit"
                endGame.draw(restartButton);
                endGame.draw(restartText);
                endGame.draw(exitButton);
                endGame.draw(exitText);

                // Afficher la fen�tre de fin de jeu � l'�cran
                endGame.display();

            }
        }




        // Afficher l'�cran
        window.display();


    }

    return 0;
}