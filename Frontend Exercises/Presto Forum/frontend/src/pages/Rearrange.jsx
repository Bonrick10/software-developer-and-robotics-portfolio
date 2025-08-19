import React from 'react';
import axios from 'axios';
import { useParams, useNavigate } from 'react-router-dom';
import { AuthContext } from '../contexts/AuthContext';
import Card from 'react-bootstrap/Card';
import { DragDropContext, Droppable, Draggable } from 'react-beautiful-dnd';
import { Button } from 'react-bootstrap';
import { StoreContext } from '../contexts/StoreContext';

export function Rearrange () {
  const { config } = React.useContext(AuthContext);
  const { storeData } = React.useContext(StoreContext);
  const { presId } = useParams();
  const [slides, setSlides] = React.useState([]);
  React.useEffect(() => {
    const getSlides = async () => {
      try {
        const response = await axios.get('http://localhost:5005/store', config);
        const userStore = response.data.store;
        console.log(userStore);
        const presIndex = userStore.store.presentations.findIndex((currPres) => {
          return (currPres.presId.toString() === presId)
        });
        setSlides(userStore.store.presentations[presIndex].slides);
        console.log(userStore.store.presentations[presIndex].slides);
      } catch (err) {
        alert(err.response);
      }
    }
    getSlides();
  }, [])
  function handleOnDragEnd (result) {
    if (!result.destination) return;
    const item = Array.from(slides);
    const [reorderItem] = item.splice(result.source.index, 1)
    item.splice(result.destination.index, 0, reorderItem);
    setSlides(item);
  }
  console.log(slides);
  const navigate = useNavigate();
  const updateAndClose = async () => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const presIndex = userStore.store.presentations.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      userStore.store.presentations[presIndex].slides = slides;
      storeData(userStore);
      console.log(userStore.store.presentations[presIndex].slides);
      navigate(`/editPres/${presId}/slide/1`);
    } catch (err) {
      alert(err.response);
    }
  }
  return (
    <>
    <Button variant="danger" onClick={updateAndClose}>Back</Button>
    <DragDropContext onDragEnd={handleOnDragEnd}>
      <Droppable droppableId='characters'>
        {(provided) => (
          <ul className="characters" {...provided.droppableProps} ref={provided.innerRef}>{slides.map((slide, index) => {
            return (
              <Draggable key={slide.slideId.toString()} draggableId={slide.slideId.toString()} index={index}>
                {(provided) => (
                  <li {...provided.draggableProps} {...provided.dragHandleProps} ref={provided.innerRef}>
                    <Card style={{ width: '18rem' }} className="CardTwo">
                      <Card.Body>
                        <Card.Text> slide number: {parseInt(slide.slideId) + 1}</Card.Text>
                      </Card.Body>
                    </Card>
                  </li>
                )}
              </Draggable>
            );
          })}
          {provided.placeholder}
          </ul>
        )}
      </Droppable>
    </DragDropContext>
    </>
  )
}
