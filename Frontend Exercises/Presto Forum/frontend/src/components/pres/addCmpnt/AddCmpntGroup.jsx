import React from 'react';
import { ButtonGroup } from 'react-bootstrap';

import { AddTextModal, AddImageModal, AddVideoModal, AddCodeModal } from '.';
// TODO: make a new context for these prop params
export function AddCmpntGroup ({ activeIndex, setSlides, presId }) {
  return (
    <ButtonGroup>
      <AddTextModal activeIndex={activeIndex} setSlides={setSlides} presId={presId} />
      <AddImageModal activeIndex={activeIndex} setSlides={setSlides} presId={presId} />
      <AddVideoModal activeIndex={activeIndex} setSlides={setSlides} presId={presId} />
      <AddCodeModal activeIndex={activeIndex} setSlides={setSlides} presId={presId} />
    </ButtonGroup>
  )
}
