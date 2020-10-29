#include "LineGraph.hpp"

void LineGraph::createDot(sf::Vector2f coords)
{
	auto newDot = dot;

	// Determine dot position
	sf::Transform transform =
	    ccs.getTransform() * viewTransform.getInverseTransform() * stretchTransform.getTransform();
	coords = transform.transformPoint(coords);

	sf::Transform dotTransform;
	dotTransform.translate(coords);
	dotTransform.scale({9, 9});

	// Transform the dot to the desired coords
	for (auto& vertx : newDot)
	{
		vertx.position = dotTransform.transformPoint(vertx.position);

		mesh.append(vertx);
	}
}

void LineGraph::calculateStretchTransform(const sf::Vector2f& canvasViewSize)
{
	sf::Vector2f canvasNormalSize =
	    ccs.getInverseAxisTransform().transformPoint(canvasViewSize);

	sf::Vector2f stretchScale(
	    canvasNormalSize.x / viewRect.width,
	    -(canvasNormalSize.y / viewRect.height)
	);

	stretchTransform.setScale(stretchScale);
}

void LineGraph::updateGraph()
{
	mesh.clear();

	// Determine how big we can see taking account of the zooming
	sf::Transform transform = viewTransform.getTransform();
	sf::FloatRect viewRegion = transform.transformRect(viewRect);

	for (auto& datum : data)
	{
		Span xViewSpan {viewRegion.left, viewRegion.left + viewRegion.width};
		Span yViewSpan {viewRegion.top, viewRegion.top + viewRegion.height};

		// If not in sight (not visible), ignore it
		if (datum.x > xViewSpan.to || datum.x < xViewSpan.from ||
		        datum.y > yViewSpan.to || datum.y < yViewSpan.from)
			continue;

		// If in sight (visible), draw it
		createDot(datum);
	}

	cgrid.setViewTransform(viewTransform.getTransform());
	cgrid.setStretchTransform(stretchTransform.getTransform());

	canvas.clear(sf::Color::Cyan);
	canvas.draw(cgrid);
	canvas.draw(mesh);
	canvas.display();

	needUpdate = false;
}

LineGraph::LineGraph()
	: cgrid(ccs)
{
	ccs.setScale(1, 1);
}

LineGraph::LineGraph(sf::Vector2f position, sf::Vector2f size, sf::Vector2f scaling)
	: cgrid(ccs)
{
	sf::Transformable::setPosition(position);
	ccs.setScale(scaling);
	setViewRegion({0, 0, size.x / scaling.x, size.y / scaling.y});
	setSize(static_cast<sf::Vector2u>(size));
	setViewPosition({0, 0});
	cgrid.setViewRegion(viewRect);
	cgrid.setColor(sf::Color::Black);
}

sf::Vector2f LineGraph::getData(std::size_t index) const
{
	return data[index];
}
void LineGraph::addData(sf::Vector2f datum)
{
	data.emplace_back(datum.x, datum.y);
	needUpdate = true;
}
void LineGraph::removeData(sf::Vector2f datum)
{
	data.erase(std::remove(data.begin(), data.end(), datum), data.end());
	needUpdate = true;
}
void LineGraph::replaceData(sf::Vector2f datum, sf::Vector2f newDatum)
{
	removeData(datum);
	addData(newDatum);
	needUpdate = true;
}
void LineGraph::clearData()
{
	data.clear();
}
std::size_t LineGraph::getDataCount()
{
	return data.size();
}

// View region, Not to be confused with size of graph
sf::FloatRect LineGraph::getViewRegion() const
{
	return viewRect;
}

void LineGraph::setViewRegion(sf::FloatRect viewRegion)
{
	viewRect = viewRegion;
	cgrid.setViewRegion(viewRegion);
	needUpdate = true;
}

// Size of the graph
sf::Vector2u LineGraph::getSize() const
{
	return canvas.getSize();
}

void LineGraph::setSize(sf::Vector2u size)
{
	if (!canvas.create(size.x, size.y))
	{
		sf::err() << "Failed creating graph canvas!" << std::endl;
		return;
	}

	if (viewRect.width == 0 || viewRect.height == 0)
	{
		sf::err() << "Invalid view rect size" << std::endl;
		return;

		// Todo, generate default view instead of returning
	}

	sf::Vector2f canvasSize = static_cast<sf::Vector2f>(canvas.getSize());
	canvas.setView(sf::View({0, -canvasSize.y, canvasSize.x, canvasSize.y}));
	calculateStretchTransform(canvasSize);

	display.setTexture(canvas.getTexture(), true);
	needUpdate = true;
}

sf::Vector2f LineGraph::getZoom() const
{
	return viewTransform.getScale();
}

void LineGraph::setZoom(sf::Vector2f zoom)
{
	viewTransform.setScale({1 / zoom.x, 1 / zoom.y});
	needUpdate = true;
}

sf::Vector2f LineGraph::getViewPosition() const
{
	return viewTransform.getPosition();
}

void LineGraph::setViewPosition(const sf::Vector2f& position)
{
	viewTransform.setPosition(position);
	cgrid.setViewRegion(viewRect);
	needUpdate = true;
}

void LineGraph::moveViewPosition(const sf::Vector2f& offset)
{
	viewTransform.move(offset);
	cgrid.moveViewRegion(offset);
	needUpdate = true;
}


void LineGraph::update()
{
	if (needUpdate)
	{
		updateGraph();
	}
}

void LineGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(display, states);
}
