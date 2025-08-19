import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function EditImageBox ({ activeIndex, setSlides, presId, imageTarget, imageIndex, editImage, setEditImage }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const [width, setWidth] = React.useState(imageTarget.width);
  const [height, setHeight] = React.useState(imageTarget.height);
  const [src, setSrc] = React.useState(imageTarget.src);
  const [alt, setAlt] = React.useState(imageTarget.alt);

  const [show, setShow] = React.useState(false);

  const handleClose = () => setShow(false);
  const handleShow = () => setShow(true);

  const getBase64 = (file) => {
    const reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onload = function () {
      setSrc(reader.result);
    };
    reader.onerror = (error) => {
      alert('Error: ', error);
    };
  }

  const EditImage = async () => {
    if (!width || !height || !src || !alt) {
      alert('Please fill empty fields');
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
      const currImage = currSlide.image[imageIndex];

      currImage.width = width;
      currImage.height = height;
      currImage.src = src;
      currImage.alt = alt;

      storeData(userStore);
      setSlides(currPres.slides);
      handleClose();
      setEditImage(false);
    } catch (err) {
      alert(err.response);
    }
  }

  return (
    <>
    {editImage && (<Button variant="secondary" onClick={handleShow}>Edit Image</Button>)}
    <Modal
        show={show}
        onHide={handleClose}
        backdrop="static"
        keyboard={false}
      >
        <Modal.Header closeButton>
          <Modal.Title>Edit Image</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Form>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Width</Form.Label>
              <Form.Control
                type="number"
                defaultValue={imageTarget.width}
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
                defaultValue={imageTarget.height}
                min="1"
                max="100"
                step=".1"
                placeholder="New height %"
                onChange={e => setHeight(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Image Source</Form.Label>
              <Form.Control
                type="file"
                onChange={e => getBase64(e.target.files[0])}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Alt Text</Form.Label>
              <Form.Control
                type="text"
                defaultValue={imageTarget.alt}
                placeholder="Alternate text for the image"
                onChange={e => setAlt(e.target.value)}
                autoFocus
              />
            </Form.Group>
          </Form>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="danger" onClick={handleClose}>
            Cancel
          </Button>
          <Button variant="primary" onClick={EditImage}>Edit</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
