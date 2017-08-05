#pragma once
class SpriteObject
{
public:
	SpriteObject();
	virtual ~SpriteObject();
	virtual bool IsLoaded() const;
	virtual void Load(std::string filename);
	virtual void Draw(sf::RenderWindow & window);
	virtual void Update(float elapsedTime);	
	virtual void SetPosition(float x, float y);
	virtual sf::Vector2f GetPosition() const;
	virtual float GetWidth() const;
	virtual float GetHeight() const;
	virtual sf::Vector2f GetPreviousPosition() const;
	virtual void SetPreviousPosition(sf::Vector2f newPosition);
	virtual sf::Rect<float> GetBoundingRect() const;
	sf::Sprite& GetSprite();
protected:
	
	
private:
	sf::Sprite  _sprite;
	sf::Texture _image;
	std::string _filename;
	bool _isLoaded;


};