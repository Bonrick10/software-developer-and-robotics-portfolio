import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function EditVideoBox ({ activeIndex, setSlides, presId, videoTarget, videoIndex, editVideo, setEditVideo }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const [URL, setURL] = React.useState(videoTarget.URL);
  const [width, setWidth] = React.useState(videoTarget.width);
  const [height, setHeight] = React.useState(videoTarget.height);
  const [autoPlay, setAutoPlay] = React.useState(videoTarget.autoPlay);

  const handleSwitchChange = (event) => {
    setAutoPlay(event.target.checked);
  };

  const [show, setShow] = React.useState(false);

  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);

  const EditVideo = async () => {
    if (!width || !height || !URL) {
      alert('Please fill out empty fields');
    }
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const userPres = userStore.store.presentations;
      const presIndex = userPres.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      const currPres = userPres[presIndex];
      const currSlide = currPres.slides[activeIndex];
      const currVideo = currSlide.video[videoIndex];

      currVideo.width = width;
      currVideo.height = height;
      currVideo.URL = URL;
      currVideo.autoPlay = autoPlay;

      storeData(userStore);
      setSlides(currPres.slides);
      handleClose();
      setEditVideo(false);
    } catch (err) {
      alert(err.response);
    }
  }

  const enterKeyDown = (event) => {
    if (event.key === 'Enter') {
      EditVideo();
    }
  }

  return (
    <>
      {editVideo && (<Button variant="secondary" onClick={handleShow}>Edit Video</Button>)}
      <Modal
        show={show}
        onHide={handleClose}
        backdrop="static"
        keyboard={false}
      >
        <Modal.Header closeButton>
          <Modal.Title>Edit Video</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Form>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Width</Form.Label>
              <Form.Control
                type="number"
                defaultValue={videoTarget.width}
                min="1"
                max="100"
                step=".1"
                placeholder="New width %"
                onChange={e => setWidth(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Height</Form.Label>
              <Form.Control
                type="number"
                defaultValue={videoTarget.height}
                min="1"
                max="100"
                step=".1"
                placeholder="New height %"
                onChange={e => setHeight(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>URL</Form.Label>
              <Form.Control
                type="URL"
                defaultValue={videoTarget.URL}
                placeholder="New video URL"
                onChange={e => setURL(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Check
              type="switch"
              id="custom-switch"
              label="Auto-play"
              checked={autoPlay}
              onChange={handleSwitchChange}
            />
          </Form>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="danger" onClick={handleClose}>
            Cancel
          </Button>
          <Button variant="primary" onKeyDown={enterKeyDown} onClick={EditVideo}>Edit</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
